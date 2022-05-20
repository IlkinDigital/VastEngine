#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"

#include <imgui.h>

#include "EditorLayout/Layout.h"

namespace Vast {

	void EditorLayer::OnAttach()
	{
		Window& window = Application::Get().GetWindow();
		m_Camera = CreateRef<PerspectiveCamera>(1.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.01f, 1000.0f);

		m_Framebuffer = Framebuffer::Create({ window.GetWidth(), window.GetHeight() });

		m_PatrickTexture = Texture2D::Create("Assets/Textures/PatrickAlpha.png");

		m_ActiveScene = CreateRef<Scene>();

		Entity camera = m_ActiveScene->CreateEntity("Omni camera");
		Entity box1 = m_ActiveScene->CreateEntity("Red square");
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
			m_Camera->SetProjection(1.0f, aspectViewport, 0.01f, 1000.0f);
			m_ActiveScene->OnViewportResize(m_Viewport.GetWidth(), m_Viewport.GetHeight());
		}

		//Vector3& pos = m_CameraPosition;

		//if (m_Viewport.IsHovered())
		//{
		//	if (Input::IsPressed(Mouse::Right))
		//	{
		//		if (Input::IsPressed(Key::W))
		//			pos.y += m_CameraSpeed * ts;
		//		if (Input::IsPressed(Key::S))
		//			pos.y -= m_CameraSpeed * ts;
		//		if (Input::IsPressed(Key::D))
		//			pos.x += m_CameraSpeed * ts;
		//		if (Input::IsPressed(Key::A))
		//			pos.x -= m_CameraSpeed * ts;
		//	}
		//}

		//m_Camera->SetPosition(pos);
		//m_Camera->SetRotation(m_CameraRotation);

		m_Framebuffer->Bind();

		RendererCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGUIRender()
	{
		EditorLayout::BeginDockspace();

		ImGui::Begin("Settings");
		
		ImGui::BeginGroup();
		
		ImGui::Text("FPS: %.3f", m_FPS);

		ImGui::EndGroup();
		
		ImGui::DragFloat("Pos x", &m_CameraPosition.x, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);
		ImGui::DragFloat("Pos y", &m_CameraPosition.y, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);
		ImGui::DragFloat("Pos z", &m_CameraPosition.z, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);		
		ImGui::DragFloat("Rot x", &m_CameraRotation.x, 0.1f, -360.0f, 360.0f, "%.f", 1.0f);
		ImGui::DragFloat("Rot y", &m_CameraRotation.y, 0.1f, -360.0f, 360.0f, "%.f", 1.0f);
		ImGui::DragFloat("Rot z", &m_CameraRotation.z, 0.1f, -360.0f, 360.0f, "%.f", 1.0f);
		
		ImGui::End();

		m_Viewport.OnGUIRender(m_Framebuffer->GetColorAttachment());

		EditorLayout::EndDockspace();
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnEvent(Event& event)
	{
	}

}