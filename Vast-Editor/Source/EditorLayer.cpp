#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"

#include <imgui.h>

#include "EditorLayout/Layout.h"
#include "EditorCore/EditorControl.h"

#include "NativeScripting/CodeGenerator.h"

namespace Vast {

	typedef void(*ScrptFunc)(Entity);
	typedef void(*InitFunc)(Application*);

	InitFunc InitScriptFunc;
	ScrptFunc AddScriptFunc;

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

		OpenScene("Assets/Scenes/TestScene2.vast");

		CodeGenerator gen(m_Project);
		gen.GenerateProjectFile();

#if 0
		class CharacterController : public ScriptableEntity
		{
		public:
			void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>();
				
				if (Input::IsPressed(Key::W))
					transform.Translation.y += m_Speed * ts;
				if (Input::IsPressed(Key::S))
					transform.Translation.y -= m_Speed * ts;
				if (Input::IsPressed(Key::D))
					transform.Translation.x += m_Speed * ts;
				if (Input::IsPressed(Key::A))
					transform.Translation.x -= m_Speed * ts;
			}
		private:
			float m_Speed = 3.0f;
		};

		class FollowCamera : public ScriptableEntity
		{
		public:
			void OnCreate() override
			{
				UUID id = 7326193114781319049;
				m_TargetEntity = GetEntity(id);
			}

			void OnUpdate(Timestep ts)
			{
				auto& cameraPos = GetComponent<TransformComponent>().Translation;
				auto& targetPos = m_TargetEntity.GetComponent<TransformComponent>().Translation;

				cameraPos.x = targetPos.x;
				cameraPos.y = targetPos.y;
			}

		private:
			Entity m_TargetEntity;
		};

		auto characterView = m_ActiveScene->GetRegistry().view<RenderComponent>();
		auto cameraView = m_ActiveScene->GetRegistry().view<CameraComponent>();

		for (auto entity : characterView)
		{
			if (m_ActiveScene->GetRegistry().get<TagComponent>(entity).Tag == "Patrick Star")
			{
				m_ActiveScene->GetRegistry().emplace<NativeScriptComponent>(entity).Bind<CharacterController>();
				for (auto camera : cameraView)
					m_ActiveScene->GetRegistry().emplace<NativeScriptComponent>(camera).Bind<FollowCamera>();
				break;
			}
		}
#endif

#if 0
		m_PatrickTexture = Texture2D::Create("Assets/Textures/PatrickAlpha.png");
		m_BGTexture = Texture2D::Create("Assets/Textures/magic-cliffs-preview-detail.png");

		Entity camera = m_ActiveScene->CreateEntity("Omni camera");
		Entity box2 = m_ActiveScene->CreateEntity("Blue square");
		Entity box1 = m_ActiveScene->CreateEntity("Patrick Star");
		Entity bg = m_ActiveScene->CreateEntity("Background");

		auto& cc = camera.AddComponent<CameraComponent>();
		cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
		auto& tc = camera.GetComponent<TransformComponent>();
		tc.Translation.z = 5.0f;
		tc.Translation.x = 2.0f;

		box1.AddComponent<RenderComponent>(m_PatrickTexture);
		box1.GetComponent<TransformComponent>().Translation = { 0.5f, 1.0f, 1.0f };
		box1.GetComponent<TransformComponent>().Scale = { 1.0f, 1.7f, 1.0f };

		bg.AddComponent<RenderComponent>(m_BGTexture);
		bg.GetComponent<TransformComponent>().Translation = { 0.0f, 0.0f, 0.6f };
		bg.GetComponent<TransformComponent>().Scale = { 16.0f, 6.0f, 6.0f };

		auto& rc2 = box2.AddComponent<RenderComponent>();
		rc2.Color = { 0.2f, 0.4f, 0.8f, 0.5f };
		box2.GetComponent<TransformComponent>().Translation = { 0.0f, 0.0f, 2.0f };

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate() override
			{
			}

			void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>();
				auto& pos = GetComponent<TransformComponent>().Translation;

				Vector2 mouse = Input::GetMousePosition();
				Vector2 delta = (mouse - m_InitialPos) * 0.3f;
				m_InitialPos = mouse;

				if (Input::IsPressed(Mouse::Right))
				{
					float yaw = transform.Rotation.y;
					float pitch = transform.Rotation.x;
					yaw -= m_RotSpeed * delta.x * ts;
					pitch -= m_RotSpeed * delta.y * ts;
					transform.Rotation = { pitch, yaw, 0.0f };
				}
			}
		private:
			Vector2 m_InitialPos;
			float m_RotSpeed = 5.0f;
		};

		auto group = m_ActiveScene->GetRegistry().view<CameraComponent>();

		for (auto entity : group)
		{
			m_ActiveScene->GetRegistry().emplace<NativeScriptComponent>(entity).Bind<CameraController>();
		}
#endif

	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_FPSWait += ts;
		if (m_FPSWait > 0.1f)
		{
			m_FPS = 1.0f / ts;
			m_FPSWait = 0.0f;
		}


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
				if (ImGui::MenuItem("New", "Ctrl + N"))
					NewScene();
				if (ImGui::MenuItem("Open", "Ctrl + O"))
					OpenScene(FileIO::Dialogs::OpenFile("Vast Scene (*.vast)\0*.vast\0"));
				if (ImGui::MenuItem("Save As", "Ctrl + Alt + S"))
					SaveScene(FileIO::Dialogs::SaveFile("Vast Scene (*.vast)\0*.vast\0"));

				ImGui::EndMenu();
			}

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
		
		ImGui::Text("FPS: %.3f", m_FPS);
		
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
		m_ScriptModule = RuntimeModule::Create(m_Project.GetScriptModulePath());

		InitScriptFunc = m_ScriptModule->LoadFunction<InitFunc>("Init");
		AddScriptFunc = m_ScriptModule->LoadFunction<ScrptFunc>("AddNativeScript");

		InitScriptFunc(Application::GetPointer());

		auto characterView = m_ActiveScene->GetRegistry().view<RenderComponent>();

		for (auto entityID : characterView)
		{
			if (m_ActiveScene->GetRegistry().get<TagComponent>(entityID).Tag == "Patrick Star")
			{
				Entity entity(entityID, m_ActiveScene.get());
				AddScriptFunc(entity);
				break;
			}
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

	void EditorLayer::OpenScene(const String& filepath)
	{
		if (!filepath.empty())
		{
			if (m_SceneState == SceneState::Play)
				OnSceneStop();
			
			m_EditorScene = CreateRef<Scene>();
			SceneSerializer serializer(m_EditorScene);
			serializer.Deserialize(filepath);
			m_SceneFilepath = filepath;

			m_ActiveScene = m_EditorScene;

			m_Lineup.SetContext(m_ActiveScene);
			m_Gizmo.UpdateData({}, m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetViewProjection());
			ResizeViewport();
		}
	}

	void EditorLayer::SaveScene(const String& filepath)
	{
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_EditorScene);
			serializer.Serialize(filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_RuntimeScene = Scene::Clone(m_EditorScene);
		
		m_ActiveScene = m_RuntimeScene;
		m_Lineup.SetContext(m_ActiveScene);

		ResizeViewport();
		UpdateScriptModule();
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		
		m_RuntimeScene = nullptr;

		m_ActiveScene = m_EditorScene;
		m_Lineup.SetContext(m_ActiveScene);

		m_ScriptModule->Clean();
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