#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"

#include <imgui.h>

namespace Vast {

	void EditorLayer::OnAttach()
	{
		Window& window = Application::Get().GetWindow();
		m_Camera = CreateRef<PerspectiveCamera>(1.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.01f, 1000.0f);

		m_Framebuffer = Framebuffer::Create({ window.GetWidth(), window.GetHeight() });

		m_PatrickTexture = Texture2D::Create("Assets/Textures/PatrickAlpha.png");
	}

	void EditorLayer::OnGUIRender()
	{
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

		uint32 textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)textureID, ImVec2{ 640.0f, 360.0f });
		
		ImGui::End();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_FPSWait += ts;
		if (m_FPSWait > 0.1f)
		{
			m_FPS = 1.0f / ts;
			m_FPSWait = 0.0f;
		}

		Vector3& pos = m_CameraPosition;

		if (Input::IsPressed(Mouse::Right))
		{
			if (Input::IsPressed(Key::W))
				pos.y += m_CameraSpeed * ts;
			if (Input::IsPressed(Key::S))
				pos.y -= m_CameraSpeed * ts;
			if (Input::IsPressed(Key::D))
				pos.x += m_CameraSpeed * ts;
			if (Input::IsPressed(Key::A))
				pos.x -= m_CameraSpeed * ts;
		}

		m_Camera->SetPosition(pos);
		m_Camera->SetRotation(m_CameraRotation);

		m_Framebuffer->Bind();
		RendererCommand::Clear();

		Renderer2D::BeginScene(*m_Camera, m_Camera->GetView());

		Renderer2D::DrawQuad(Math::Transform(
			{ -0.5f, -0.5f, 0.0f }, // translation
			{ 0.0f, 0.0f, Math::Radians(45.0f) },   // rotation
			{ 1.0f, 1.0f, 1.0f }    // sacale
		), { 0.2f, 0.8f, 0.5f, 1.0f });

		Renderer2D::DrawQuad(Math::Transform(
			{ 1.0f, 1.0f, 2.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.5f, 1.0f }
		), m_PatrickTexture);

		Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}


	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(VAST_BIND_EVENT(OnWindowResize));
	}

	bool EditorLayer::OnWindowResize(WindowResizeEvent& event)
	{
		RendererCommand::SetViewport(0.0f, 0.0f, event.GetWidth(), event.GetHeight());
		float aspect = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera->SetProjection(1.0f, aspect, 0.01f, 1000.0f);
		return false;
	}

}