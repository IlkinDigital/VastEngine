#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"
#include "Scripting/ScriptBuffer.h"

#include "Clock/FrameTime.h"

#include <imgui.h>

#include "EditorLayout/Layout.h"
#include "EditorCore/EditorControl.h"
#include "NativeScripting/CodeGenerator.h"

#include "Serialization/ProjectSerializer.h"
#include "Project/ProjectGenerator.h"

#include "Utils/System/System.h"

#include "Board2D/BoardFlipbook.h"
#include "Board2D/BoardStateMachine.h"

namespace Vast {

// Returns relative filepath through project's directory
#define PROJDIR(path) (m_Project->GetProjectPath() / path)

	static FrameTime s_FrameTime(100);

	typedef void(*InitScriptsFn)();
	typedef void(*InitModuleFn)(Application*);
	typedef const DArray<NativeScriptComponent>&(*GetScriptsFn)();

	static InitModuleFn InitModule;
	static InitScriptsFn InitScripts;
	static GetScriptsFn GetScripts;

	void EditorLayer::OnAttach()
	{
		Window& window = Application::Get().GetWindow();
		m_Framebuffer = Framebuffer::Create({ window.GetWidth(), window.GetHeight() });

		m_PlayIcon = Texture2D::Create("Resources/Icons/PlayIcon.png");
		m_StopIcon = Texture2D::Create("Resources/Icons/StopIcon.png");

		m_Viewport.SetDragDropFn([&](const String& filepath)
			{
				OpenScene(filepath);
			});

		OpenProject("D:/Lester_Files/dev/VastProjects/WackoDuel");
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		s_FrameTime.Update(ts);

		auto spec = m_Framebuffer->GetSpecification();
		if (spec.Width != m_Viewport.GetWidth() || spec.Height != m_Viewport.GetHeight())
		{
			ResizeViewport();
		}

		m_Framebuffer->Bind();

		RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		RendererCommand::Clear();

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

		m_Framebuffer->Unbind();
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
		EditorLayout::BeginDockspace();

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl + N"))
					NewScene();
				if (ImGui::MenuItem("Open Scene", "Ctrl + O"))
					OpenScene(FileIO::Dialogs::OpenFile("Vast Scene (*.vast)\0*.vast\0"));
				if (ImGui::MenuItem("Save Scene As", "Ctrl + Alt + S"))
					SaveScene(FileIO::Dialogs::SaveFile("Vast Scene (*.vast)\0*.vast\0"));
				if (ImGui::MenuItem("New Project"))
					NewProject("Hello_World", FileIO::Dialogs::SaveFile("Vast Project (vast.project)\0vast.project\0").parent_path());
				if (ImGui::MenuItem("Open Project"))
					OpenProject(FileIO::Dialogs::OpenFile("Vast Project (vast.project)\0vast.project\0").parent_path());

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

			float snapRot = m_Gizmo.GetSnapValues().y;
			if (ImGui::DragFloat("Rotation Snap", &snapRot))
				m_Gizmo.SetRotationSnap(snapRot);

			float snapTS = m_Gizmo.GetSnapValues().x;
			if (ImGui::DragFloat("Translation/Scale Snap", &snapTS))
				m_Gizmo.SetTrScSnap(snapTS);

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

		m_Gizmo.UpdateData(m_Lineup.GetSelected(), m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjection());

		m_Viewport.OnGUIRender(m_Framebuffer->GetColorAttachment(), m_Gizmo);
		m_Lineup.OnGUIRender();
		m_Properties.OnGUIRender(m_Lineup.GetSelected());

		m_ContentBrowser.OnGUIRender();

		ImGui::ShowDemoWindow((bool*)1);

		EditorLayout::EndDockspace();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(VAST_BIND_EVENT(OnKeyPressed));
	}

	void EditorLayer::ResizeViewport()
	{
		if (m_Viewport.GetWidth() > 0 && m_Viewport.GetHeight() > 0)
		{
			m_Framebuffer->Resize({ m_Viewport.GetWidth(), m_Viewport.GetHeight() });
			
			float aspectViewport = (float)m_Viewport.GetWidth() / (float)m_Viewport.GetHeight();
			m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
			m_EditorCamera.SetViewportSize((float)m_Viewport.GetWidth(), (float)m_Viewport.GetHeight());
		}
	}

	void EditorLayer::UpdateScriptModule()
	{
		m_ScriptModule = RuntimeModule::Create(m_Project->GetScriptModulePath());

		if (m_ScriptModule->IsLoaded())
		{
			InitModule = m_ScriptModule->LoadFunction<InitModuleFn>("InitModule");
			InitScripts = m_ScriptModule->LoadFunction<InitScriptsFn>("InitScripts");
			GetScripts = m_ScriptModule->LoadFunction<GetScriptsFn>("GetScripts");

			InitModule(Application::GetPointer());
			InitScripts();
			ScriptBuffer::Get().SetBuffer(GetScripts());
		}
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_ActiveScene = CreateRef<Scene>();
		m_EditorScene = m_ActiveScene;
		m_Lineup.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene(const Filepath& filepath)
	{
		if (std::filesystem::exists(filepath))
		{
			if (m_SceneState == SceneState::Play)
				OnSceneStop();

			m_EditorScene = CreateRef<Scene>();
			SceneSerializer serializer(m_EditorScene);
			serializer.Deserialize(filepath.string());
			m_SceneFilepath = filepath;

			m_ActiveScene = m_EditorScene;

			m_Lineup.SetContext(m_ActiveScene);
			m_Gizmo.UpdateData({}, m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetViewProjection());
			ResizeViewport();
		}
		else
			VAST_ERROR("Invalid path to scene '{0}'", filepath.string());
	}

	void EditorLayer::SaveScene(const Filepath& filepath)
	{
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_EditorScene);
			serializer.Serialize(filepath.string());
		}
	}

	void EditorLayer::OpenProject(const Filepath& filepath)
	{
		bool first = !(bool)m_Project;
		String name;
		if (!first)
			name = m_Project->GetName();

		m_Project = CreateRef<Project>();

		ProjectSerializer ps(m_Project);
		ps.Deserialize(filepath);

		m_ContentBrowser.SetRootDirectory(PROJDIR("Content"));

		if (name != m_Project->GetName())
		{
			if (m_ScriptModule)
				m_ScriptModule->Clear();
			ScriptBuffer::Get().ClearBuffer();
			UpdateScriptModule();
		}

		// TODO: Open last opened scene registered by .ini
		NewScene();
	}

	void EditorLayer::NewProject(const String& name, const Filepath& filepath)
	{
		m_Project = CreateRef<Project>(name, filepath);
		ProjectSerializer ps(m_Project);
		ps.Serialize(filepath);

		ProjectGenerator pg(m_Project);
		pg.GenerateDirectories();
		pg.GeneratePremakeFile();
		pg.DownloadDependencies(PROJDIR("Engine"));

		CodeGenerator gen(m_Project);
		gen.GeneratePCH();
		gen.GenerateExportFiles();

		RunPremake();

		m_ContentBrowser.SetRootDirectory(PROJDIR("Content"));
		m_ScriptModule->Clear();
		ScriptBuffer::Get().ClearBuffer();
	}

	void EditorLayer::BuildScripts()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		CodeGenerator gen(m_Project);
		gen.GenerateExportFiles();
		
		if (std::filesystem::exists(m_Project->GetScriptModulePath().root_directory()))
		{
			m_ScriptModule->Clear();
			m_Project->Build();
			UpdateScriptModule();
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
		m_SceneState = SceneState::Play;

		m_RuntimeScene = Scene::Clone(m_EditorScene);
		
		m_ActiveScene = m_RuntimeScene;
		m_Lineup.SetContext(m_ActiveScene);

		ResizeViewport();
	}

	void EditorLayer::OnSceneStop()
	{
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
					m_Gizmo.SetGizmoType(Gizmo3D::GizmoType::None);
					break;
				case Key::W:
					m_Gizmo.SetGizmoType(Gizmo3D::GizmoType::Translation);
					break;
				case Key::E:
					m_Gizmo.SetGizmoType(Gizmo3D::GizmoType::Rotation);
					break;
				case Key::R:
					m_Gizmo.SetGizmoType(Gizmo3D::GizmoType::Scale);
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
				OpenScene(FileIO::Dialogs::OpenFile("Vast Scene (*.vast)\0*.vast\0"));
			break;
		case Key::S:
			if (Input::IsPressed(Key::LeftControl))
			{
				if (Input::IsPressed(Key::LeftAlt))
					SaveScene(FileIO::Dialogs::SaveFile("Vast Scene (*.vast)\0*.vast\0"));
				else
				{
					if (!m_SceneFilepath.empty())
						SaveScene(m_SceneFilepath);
					else
						SaveScene(FileIO::Dialogs::SaveFile("Vast Scene (*.vast)\0*.vast\0"));
				}
			}
			break;
		case Key::D:
			if (Input::IsPressed(Key::LeftControl))
			{
				Entity selected = m_Lineup.GetSelected();
				if (selected.IsValid())
					m_ActiveScene->DuplicateEntity(selected);
			}
			break;
		}


		return false;
	}

}