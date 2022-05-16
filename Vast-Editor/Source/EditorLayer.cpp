#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"

#include <imgui.h>

namespace Vast {

	void EditorLayer::OnAttach()
	{
		Window& window = Application::Get().GetWindow();
		m_Camera = CreateRef<PerspectiveCamera>(1.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.01f, 1000.0f);
	}

	void EditorLayer::OnGUIRender()
	{
		ImGui::Begin("Settings");
		ImGui::DragFloat("Pos x", &m_CameraPosition.x, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);
		ImGui::DragFloat("Pos y", &m_CameraPosition.y, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);
		ImGui::DragFloat("Pos z", &m_CameraPosition.z, 0.1f, -10.0f,  10.0f, "%.f", 1.0f);		
		ImGui::DragFloat("Rot x", &m_CameraRotation.x, 0.1f,   0.0f, 360.0f, "%.f", 1.0f);
		ImGui::DragFloat("Rot y", &m_CameraRotation.y, 0.1f,   0.0f, 360.0f, "%.f", 1.0f);
		ImGui::DragFloat("Rot z", &m_CameraRotation.z, 0.1f,   0.0f, 360.0f, "%.f", 1.0f);
		ImGui::End();
	}

	void EditorLayer::OnUpdate()
	{
		Vector3& pos = m_CameraPosition;

		if (Input::IsPressed(Key::W))
			pos.y += m_CameraSpeed;
		if (Input::IsPressed(Key::S))
			pos.y -= m_CameraSpeed;
		if (Input::IsPressed(Key::D))
			pos.x += m_CameraSpeed;
		if (Input::IsPressed(Key::A))
			pos.x -= m_CameraSpeed;

		m_Camera->SetPosition(pos);
		m_Camera->SetRotation(m_CameraRotation);

		Renderer2D::BeginScene(*m_Camera, m_Camera->GetView());

		Renderer2D::DrawQuad(Math::Transform(
			{ -0.5f, -0.5f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f }
		), { 0.2f, 0.8f, 0.5f, 1.0f });

		Renderer2D::DrawQuad(Math::Transform(
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f }
		), { 0.5f, 0.3f, 0.7f, 1.0f });

		Renderer2D::EndScene();

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