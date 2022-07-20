#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"
#include "Scripting/ScriptBuffer.h"

#include "Clock/FrameTime.h"

#include "EditorLayout/Layout.h"
#include "EditorCore/EditorControl.h"

#include "Serialization/ProjectSerializer.h"
#include "Project/ProjectGenerator.h"

#include "Utils/System/System.h"
#include "Utils/FileIO/FileIO.h"

#include "Board2D/BoardFlipbook.h"
#include "Board2D/BoardStateMachine.h"

#include "Clock/Clock.h"

#include "AssetManager/AssetTypes.h"
#include "AssetManager/AssetImporter.h"
#include "Serialization/AssetSerializer.h"

#include <imgui.h>
#include <fstream>
#include <optick.h>

namespace Vast {

// Returns relative filepath through project's directory
#define PROJDIR(path) (m_Project->GetProjectPath() / path)

	EditorLayer* EditorLayer::s_Instance = nullptr;

	static FrameTime s_FrameTime(100);

	void EditorLayer::OnAttach()
	{
		OPTICK_EVENT();

		m_SceneRenderer.Init(0, 0);

		m_PlayIcon = Texture2D::Create("Resources/Icons/PlayIcon.png");
		m_StopIcon = Texture2D::Create("Resources/Icons/StopIcon.png");

		m_Gizmo = CreateRef<Gizmo3D>();

		m_Viewport.Open();
		m_Viewport.SetDragDropFn([&](const String& filepath) { OpenScene(filepath); });
		m_Viewport.SetColorAttachment(m_SceneRenderer.GetFramebuffer()->GetColorAttachment());
		m_Viewport.SetGizmo(m_Gizmo);

		m_Lineup.Open();
		m_Properties.Open();
		m_Properties.SetContextEntity(m_Lineup.GetSelectedEntity());
		m_ContentBrowser.Open();

		OpenProject("D:/Lester_Files/dev/VastProjects/WackoDuel");

		const auto& assetManager = m_Project->GetAssetManager();

		assetManager->SetProject(m_Project);
		assetManager->Init();
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		OPTICK_EVENT();

		s_FrameTime.Update(ts);

		m_SubwindowManager.OnUpdate(ts);

		auto spec = m_SceneRenderer.GetFramebuffer()->GetSpecification();
		if (spec.Width != m_Viewport.GetWidth() || spec.Height != m_Viewport.GetHeight())
		{
			ResizeViewport();
		}

		m_SceneRenderer.Begin();

		switch (m_SceneState)
		{
		case SceneState::Edit:
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdate(ts, m_EditorCamera);
			break;
		case SceneState::Play:
			m_ActiveScene->OnRuntimeUpdate(ts);
			break;
		}

		m_SceneRenderer.End();
	}

	// For debug color palette purpose only
	/*static void DrawColor(const char* label, ImVec4& color)
	{
		float colorPtr[4]{};

		colorPtr[0] = color.x;
		colorPtr[1] = color.y;
		colorPtr[2] = color.z;
		colorPtr[3] = color.w;

		if (ImGui::ColorEdit4(label, colorPtr))
		{
			color = { colorPtr[0], colorPtr[1], colorPtr[2], colorPtr[3] };
		}
	}*/

	void EditorLayer::OnGUIRender()
	{
		OPTICK_EVENT();

		EditorLayout::BeginDockspace("Editor Dockspace");

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl + N"))
					NewScene();
				if (ImGui::MenuItem("Open Scene", "Ctrl + O"))
					OpenScene(FileIO::Relative(FileDialog::OpenFile("Vast Scene (*.asset)\0*.asset\0"), m_Project->GetContentFolderPath()));
				if (ImGui::MenuItem("Save Scene As", "Ctrl + Alt + S"))
					SaveScene(FileIO::Relative(FileDialog::SaveFile("Vast Scene (*.asset)\0*.asset\0"), m_Project->GetContentFolderPath()));
				if (ImGui::MenuItem("New Project"))
					NewProject("Hello_World", FileDialog::SaveFile("Vast Project (vast.project)\0vast.project\0").parent_path());
				if (ImGui::MenuItem("Open Project"))
					OpenProject(FileDialog::OpenFile("Vast Project (vast.project)\0vast.project\0").parent_path());

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Viewport"))
					m_Viewport.Open();
				if (ImGui::MenuItem("Lineup"))
					m_Lineup.Open();
				if (ImGui::MenuItem("Properties"))
					m_Properties.Open();
				if (ImGui::MenuItem("Content Browser"))
					m_ContentBrowser.Open();

				ImGui::EndMenu();
			}

			ImGui::Text("%s", m_Project->GetName().c_str());

			ImGui::EndMenuBar();
		}
		
		// Editor theme explorer
		/*ImGui::Begin("Color palette");

		DrawColor("TitleBg", m_Colors[ImGuiCol_TitleBg]);
		DrawColor("TitleBgActive", m_Colors[ImGuiCol_TitleBgActive]);
		DrawColor("TitleBgCollapsed", m_Colors[ImGuiCol_TitleBgCollapsed]);

		DrawColor("WindowBg", m_Colors[ImGuiCol_WindowBg]);

		// Headers
		DrawColor("Header", m_Colors[ImGuiCol_Header]);
		DrawColor("HeaderHovered", m_Colors[ImGuiCol_HeaderHovered]);
		DrawColor("HeaderActive", m_Colors[ImGuiCol_HeaderActive]);
		// Buttons
		DrawColor("Button", m_Colors[ImGuiCol_Button]);
		DrawColor("ButtonHovered", m_Colors[ImGuiCol_ButtonHovered]);
		DrawColor("HeaderActive", m_Colors[ImGuiCol_ButtonActive]);
		// Frame BG
		DrawColor("FrameBg", m_Colors[ImGuiCol_FrameBg]);
		DrawColor("FrameBgHovered", m_Colors[ImGuiCol_FrameBgHovered]);
		DrawColor("FrameBgActive", m_Colors[ImGuiCol_FrameBgActive]);
		// Tabs
		DrawColor("Tab", m_Colors[ImGuiCol_Tab]);
		DrawColor("TabHovered", m_Colors[ImGuiCol_TabHovered]);
		DrawColor("TabActive", m_Colors[ImGuiCol_TabActive]);
		DrawColor("TabUnfocused", m_Colors[ImGuiCol_TabUnfocused]);
		DrawColor("TabUnfocusedActive", m_Colors[ImGuiCol_TabUnfocusedActive]);

		ImGui::End();*/

		// Editor Camera
		ImGui::Begin("Editor Camera");
		{ 
			float gazeSpeed = m_EditorCamera.GetGazeSpeed();
			if (ImGui::DragFloat("Camera Gazing Speed", &gazeSpeed, 0.25f, 1.0f, 10.0f))
				m_EditorCamera.SetGazeSpeed(gazeSpeed);

			float snapRot = m_Gizmo->GetSnapValues().y;
			if (ImGui::DragFloat("Rotation Snap", &snapRot))
				m_Gizmo->SetRotationSnap(snapRot);

			float snapTS = m_Gizmo->GetSnapValues().x;
			if (ImGui::DragFloat("Translation/Scale Snap", &snapTS))
				m_Gizmo->SetTrScSnap(snapTS);

			ImGui::End();
		}

		// Toolbar
		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar 
			| ImGuiWindowFlags_NoScrollWithMouse);
		{
			if (ImGui::Button("Build"))
				BuildScripts();
			ImGui::SameLine();
			if (ImGui::Button("Run premake"))
				RunPremake();

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 0.0f });
			float size = ImGui::GetWindowHeight() - 4.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
			if (m_SceneState == SceneState::Edit)
			{
				if (ImGui::ImageButton((ImTextureID)m_PlayIcon->GetRendererID(), { 32, 32 }, 
					{ 0, 1 }, {1, 0}, -1, { 1, 1, 1, 1 }))
					OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play)
			{
				if (ImGui::ImageButton((ImTextureID)m_StopIcon->GetRendererID(), { 32, 32 },
					{ 0, 1 }, { 1, 0 }, -1, { 1, 1, 1, 1 }))
					OnSceneStop();
			}
			ImGui::PopStyleColor();


			ImGui::End();
		}

		ImGui::Begin("Settings");
		
		ImGui::Text("%d FPS", s_FrameTime.GetFPS());
		ImGui::Text("Frame Time: %.4f ms", s_FrameTime.GetFrameTime());
		
		ImGui::End();

		m_Gizmo->UpdateData(*m_Lineup.GetSelectedEntity(), m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjection());

		m_SubwindowManager.OnGUIRender();

		m_Viewport.OnGUIRender();
		m_Lineup.OnGUIRender();
		m_Properties.OnGUIRender();
		m_ContentBrowser.OnGUIRender();

		ImGui::ShowDemoWindow((bool*)1);

		EditorLayout::EndDockspace();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnEvent(Event& event)
	{
		OPTICK_EVENT();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(VAST_BIND_EVENT(OnKeyPressed));
		dispatcher.Dispatch<FilesDropEvent>(VAST_BIND_EVENT(OnFilesDrop));

		m_ActiveScene->OnEvent(event);
	}

	void EditorLayer::OpenFlipbookEditor(const Ref<BoardFlipbookAsset>& bfa)
	{
		OPTICK_EVENT();

		if (m_SubwindowManager.HasStorageWithUUID(bfa->GetUUID()))
		{
			VAST_ERROR("Couldn't open {0} flipbook, it's already open", bfa->GetName());
			return;
		}

		auto fbe = CreateRef<FlipbookEditor>();
		fbe->SetFlipbook(bfa);
		m_SubwindowManager.PushSubwindow(fbe, bfa->GetUUID());
	}

	void EditorLayer::ResizeViewport()
	{
		OPTICK_EVENT();

		if (m_Viewport.GetWidth() > 0 && m_Viewport.GetHeight() > 0)
		{
			m_SceneRenderer.GetFramebuffer()->Resize({m_Viewport.GetWidth(), m_Viewport.GetHeight()});
			
			float aspectViewport = (float)m_Viewport.GetWidth() / (float)m_Viewport.GetHeight();
			m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
			m_EditorCamera.SetViewportSize((float)m_Viewport.GetWidth(), (float)m_Viewport.GetHeight());
		}
	}

	void EditorLayer::NewScene()
	{
		OPTICK_EVENT();

		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_ActiveScene = CreateRef<Scene>();
		m_EditorScene = m_ActiveScene;
		m_Lineup.SetContext(m_ActiveScene);
		m_Lineup.DeselectEntity();
	}

	void EditorLayer::OpenScene(const Filepath& path)
	{
		OPTICK_EVENT();

		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += path.string() + ".asset";
		if (std::filesystem::exists(fullPath))
		{
			if (m_SceneState == SceneState::Play)
				OnSceneStop();

			m_Lineup.DeselectEntity();

			AssetSerializer as(m_Project, m_Project->GetAssetManager()->GetAsset(path));
			as.Deserialize(path);
			m_EditorScene = RefCast<SceneAsset>(as.GetAsset())->GetScene();

			m_SceneFilepath = path;

			m_ActiveScene = m_EditorScene;

			m_Lineup.SetContext(m_ActiveScene);
			m_Gizmo->UpdateData({}, m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetViewProjection());
			ResizeViewport();
		}
		else
			VAST_ERROR("Invalid path to scene '{0}'", fullPath.string());
	}

	void EditorLayer::SaveScene(const Filepath& path)
	{
		OPTICK_EVENT();

		if (!path.empty())
		{
			Ref<SceneAsset> scene = CreateRef<SceneAsset>(path.filename().stem().string(), path, UUID());
			scene->SetScene(m_EditorScene);
			AssetSerializer as(m_Project, scene);
			as.Serialize();
		}
	}

	void EditorLayer::OpenProject(const Filepath& filepath)
	{
		OPTICK_EVENT();

		bool first = !(bool)m_Project;
		String name;
		if (!first)
			name = m_Project->GetName();

		m_Project = CreateRef<Project>();

		ProjectSerializer ps(m_Project);
		ps.Deserialize(filepath);

		m_ContentBrowser.SetProject(m_Project);

		if (name != m_Project->GetName())
		{
			m_ScriptEngine.Shutdown();
			m_ScriptEngine.SetProject(m_Project);
			m_ScriptEngine.LoadModule();
		}

		// TODO: Open last opened scene registered by .ini
		NewScene();
	}

	void EditorLayer::NewProject(const String& name, const Filepath& filepath)
	{
		OPTICK_EVENT();

		m_Project = CreateRef<Project>(name, filepath);
		ProjectSerializer ps(m_Project);
		ps.Serialize(filepath);

		ProjectGenerator pg(m_Project);
		pg.GenerateDirectories();
		pg.GeneratePremakeFile();
		pg.DownloadDependencies(PROJDIR("Engine"));

		m_ScriptEngine.SetProject(m_Project);
		m_ScriptEngine.GenerateExportFiles();

		RunPremake();

		m_ContentBrowser.SetProject(m_Project);
		m_ScriptEngine.Shutdown();
		ScriptEngine::Get()->GetScriptBuffer().ClearBuffer();
	}

	void EditorLayer::BuildScripts()
	{
		OPTICK_EVENT();

		if (m_SceneState == SceneState::Play)
			OnSceneStop();
		
		if (std::filesystem::exists(m_Project->GetScriptModulePath().root_directory()))
		{
			m_ScriptEngine.Shutdown();
			m_ScriptEngine.BuildModule();
			m_ScriptEngine.LoadModule();
		}
		else
		{
			VAST_ERROR("FAILED to load script module, invalid path to dll");
		}

	}

	void EditorLayer::RunPremake()
	{
		System::RunCommand(m_Project->GetProjectPath(), "Vendor\\premake\\premake5.exe vs2022");
	}

	void EditorLayer::OnScenePlay()
	{
		OPTICK_EVENT();

		m_SceneState = SceneState::Play;

		m_RuntimeScene = Scene::Clone(m_EditorScene);
		
		m_ActiveScene = m_RuntimeScene;
		m_Lineup.SetContext(m_ActiveScene);

		ResizeViewport();
	}

	void EditorLayer::OnSceneStop()
	{
		OPTICK_EVENT();

		m_SceneState = SceneState::Edit;
		
		m_RuntimeScene = nullptr;
		
		m_ActiveScene = m_EditorScene;
		m_Lineup.SetContext(m_ActiveScene);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (!Input::IsPressed(Mouse::Right))
		{
			switch (event.GetKeyCode())
			{
				case Key::Q:
					m_Gizmo->SetGizmoType(Gizmo3D::GizmoType::None);
					break;
				case Key::W:
					m_Gizmo->SetGizmoType(Gizmo3D::GizmoType::Translation);
					break;
				case Key::E:
					m_Gizmo->SetGizmoType(Gizmo3D::GizmoType::Rotation);
					break;
				case Key::R:
					m_Gizmo->SetGizmoType(Gizmo3D::GizmoType::Scale);
					break;
			}
		}

		switch (event.GetKeyCode())
		{
		case Key::N:
			if (Input::IsPressed(Key::LeftControl))
				NewScene();
			break;
		case Key::O:
			if (Input::IsPressed(Key::LeftControl))
				OpenScene(FileIO::Relative(FileDialog::OpenFile("Vast Scene (*.asset)\0*.asset\0"), m_Project->GetContentFolderPath()));
			break;
		case Key::S:
			if (Input::IsPressed(Key::LeftControl))
			{
				if (Input::IsPressed(Key::LeftAlt))
					SaveScene(FileIO::Relative(FileDialog::SaveFile("Vast Scene (*.asset)\0*.asset\0"), m_Project->GetContentFolderPath()));
				else
				{
					if (!m_SceneFilepath.empty())
						SaveScene(m_SceneFilepath);
					else
						SaveScene(FileIO::Relative(FileDialog::SaveFile("Vast Scene (*.asset)\0*.asset\0"), m_Project->GetContentFolderPath()));
				}
			}
			break;
		case Key::D:
			if (Input::IsPressed(Key::LeftControl))
			{
				Entity selected = *m_Lineup.GetSelectedEntity();
				if (selected.IsValid())
					m_ActiveScene->DuplicateEntity(selected);
			}
			break;
		}


		return false;
	}

	bool EditorLayer::OnFilesDrop(FilesDropEvent& event)
	{


		return false;
	}
}