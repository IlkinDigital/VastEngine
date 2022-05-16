#include "EditorLayer.h"

namespace Vast {

	void EditorLayer::OnAttach()
	{
		float vertices[7 * 3]
		{
			 0.5f, -0.5f, 0.0f, 0.9f, 0.5f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.9f, 0.5f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.5f, 0.3f, 0.9f, 1.0f
		};

		uint32 indices[3]
		{
			0, 1, 2
		};

		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float4, "Color" }
			});

		m_IndexBuffer = IndexBuffer::Create(indices, 3);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		const String vertSrc = R"(
			#version 430 core

			layout (location = 0) in vec4 a_Pos;
			layout (location = 1) in vec4 a_Color;

			out vec4 v_Color;
			uniform mat4 u_ViewProjection;

			void main()
			{
				gl_Position = u_ViewProjection * a_Pos;
				v_Color = a_Color;
			}
		)";

		const String fragSrc = R"(
			#version 430 core
			
			layout (location = 0) out vec4 color;
			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}

		)";

		m_Shader = Shader::Create("BasicShader", vertSrc, fragSrc);
		m_Shader->Bind();

		m_Camera.SetPosition({ 0.25f, 0.5f, 0.0f });
		m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjection());

		RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	}

	void EditorLayer::OnGUIRender()
	{
	}

	void EditorLayer::OnUpdate()
	{
		RendererCommand::Clear();

		const Vector3& pos = m_Camera.GetPosition();

		if (Input::IsPressed(Key::W))
			m_Camera.SetPosition({ pos.x, pos.y + m_CameraSpeed, pos.z });
		if (Input::IsPressed(Key::S))
			m_Camera.SetPosition({ pos.x, pos.y - m_CameraSpeed, pos.z });
		if (Input::IsPressed(Key::D))
			m_Camera.SetPosition({ pos.x + m_CameraSpeed, pos.y, pos.z });
		if (Input::IsPressed(Key::A))
			m_Camera.SetPosition({ pos.x - m_CameraSpeed, pos.y, pos.z });

		m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjection());

		RendererCommand::DrawIndexed(3);
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
		float width = event.GetWidth() / 1000.0f;
		float height = event.GetHeight() / 1000.0f;
		m_Camera.SetProjection(-width, width, -height, height);
		return false;
	}

}