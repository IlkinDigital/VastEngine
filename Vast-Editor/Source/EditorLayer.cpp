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

		m_ActiveScene = CreateRef<Scene>();

		m_Lineup.SetContext(m_ActiveScene);

		Entity camera = m_ActiveScene->CreateEntity("Omni camera");
		Entity box1 = m_ActiveScene->CreateEntity("Patrick Star");
		Entity box2 = m_ActiveScene->CreateEntity("Blue square");

		auto& cc = camera.AddComponent<CameraComponent>();
		cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
		auto& tc = camera.GetComponent<TransformComponent>();
		tc.Translation.z = 5.0f;
		tc.Translation.x = 2.0f;

		box1.AddComponent<RenderComponent>(m_PatrickTexture);
		box1.GetComponent<TransformComponent>().Translation = { 0.5f, 2.0f, 1.0f };
		box1.GetComponent<TransformComponent>().Scale = { 1.0f, 1.7f, 1.0f };

		auto& rc2 = box2.AddComponent<RenderComponent>();
		rc2.Color = { 0.2f, 0.4f, 0.8f, 1.0f };

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
			m_EditorCamera.SetViewportSize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
		}

		m_Framebuffer->Bind();

		RendererCommand::Clear();

		m_EditorCamera.OnUpdate(ts);

		m_ActiveScene->OnUpdate(ts, m_EditorCamera);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGUIRender()
	{
		EditorLayout::BeginDockspace();


		// Editor Camera
		ImGui::Begin("Editor Camera");

		Vector3 forward = m_EditorCamera.GetForwardDirection();
		Vector3 right = m_EditorCamera.GetRightDirection();

		EditorControl::DrawVector3("Translation", m_EditorCamera.GetPosition());
		EditorControl::DrawVector3("Forward", forward);
		EditorControl::DrawVector3("Right", right);

		ImGui::End();


		ImGui::Begin("Settings");
		
		ImGui::Text("FPS: %.3f", m_FPS);
		
		ImGui::End();

		m_Viewport.OnGUIRender(m_Framebuffer->GetColorAttachment(), m_Lineup.GetSelected(),
			m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjection());

		m_Lineup.OnGUIRender();
		m_Properties.OnGUIRender(m_Lineup.GetSelected());

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

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
		case Key::Q:
			m_Viewport.SetGizmoType(-1);
			break;
		case Key::W:
			m_Viewport.SetGizmoType(0);
			break;
		case Key::E:
			m_Viewport.SetGizmoType(2);
			break;
		case Key::R:
			m_Viewport.SetGizmoType(1);
			break;
		}

		return false;
	}

}