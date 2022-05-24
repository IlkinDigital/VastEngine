#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"

#include <imgui.h>

#include "EditorLayout/Layout.h"
#include "EditorCore/EditorControl.h"

namespace Vast {

	void EditorLayer::OnAttach()
	{
		Window& window = Application::Get().GetWindow();

		m_Framebuffer = Framebuffer::Create({ window.GetWidth(), window.GetHeight() });

		m_PatrickTexture = Texture2D::Create("Assets/Textures/PatrickAlpha.png");
		m_BGTexture = Texture2D::Create("Assets/Textures/magic-cliffs-preview-detail.png");

		m_ActiveScene = CreateRef<Scene>();

		m_Lineup.SetContext(m_ActiveScene);

		m_Viewport.SetDragDropFn([&](const String& filepath)
			{
				OpenScene(filepath);
			});

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("Assets/Scenes/TestScene.vast");
#if 0
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

		camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
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
		if (m_Viewport.GetWidth() > 0 && m_Viewport.GetHeight() > 0 && (spec.Width != m_Viewport.GetWidth() || spec.Height != m_Viewport.GetHeight()))
		{
			m_Framebuffer->Resize({ m_Viewport.GetWidth(), m_Viewport.GetHeight() });

			float aspectViewport = (float)m_Viewport.GetWidth() / (float)m_Viewport.GetHeight();
			m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
			m_EditorCamera.SetViewportSize((float)m_Viewport.GetWidth(), (float)m_Viewport.GetHeight());
		}

		m_Framebuffer->Bind();

		RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
		RendererCommand::Clear();

		m_EditorCamera.OnUpdate(ts);

		m_ActiveScene->OnUpdate(ts, m_EditorCamera);

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

			Vector3 forward = m_EditorCamera.GetForwardDirection();
			Vector3 right = m_EditorCamera.GetRightDirection();

			EditorControl::DrawVector3("Translation", m_EditorCamera.GetPosition());
			EditorControl::DrawVector3("Forward", forward);
			EditorControl::DrawVector3("Right", right);

			float snapRot = m_Gizmo.GetSnapValues().y;
			if (ImGui::DragFloat("Rotation Snap", &snapRot))
				m_Gizmo.SetRotationSnap(snapRot);

			float snapTS = m_Gizmo.GetSnapValues().x;
			if (ImGui::DragFloat("Translation/Scale Snap", &snapTS))
				m_Gizmo.SetTrScSnap(snapTS);

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

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
		m_Lineup.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene(const String& filepath)
	{
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
			m_Lineup.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveScene(const String& filepath)
	{
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
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
			if (Input::IsPressed(Key::LeftControl) && Input::IsPressed(Key::LeftAlt))
				SaveScene(FileIO::Dialogs::SaveFile("Vast Scene (*.vast)\0*.vast\0"));
			break;
		}


		return false;
	}

}