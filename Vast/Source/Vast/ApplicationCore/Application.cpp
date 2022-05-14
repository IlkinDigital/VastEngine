#include "vastpch.h"
#include "Application.h"

#include "Events/EventDispatcher.h"

#include "Renderer/RendererCommand.h"

#include <glad/glad.h>

namespace Vast {

	Application* Application::s_Instance = nullptr;


	Application::Application(const String& name)
	{
		VAST_ASSERT(!s_Instance, "Application already exists");

		s_Instance = this;

		m_Window = Window::Create({ name });
		m_Window->SetEventCallback(VAST_BIND_EVENT(OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		float vertices[7 * 3]
		{
			 0.5f, -0.5f, 0.0f, 0.9f, 0.5f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.9f, 0.5f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.5f, 0.3f, 0.9f, 1.0f
		};

		GLuint indices[3]
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

			void main()
			{
				gl_Position = a_Pos;
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

		RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	}

	Application::~Application()
	{
		if (m_Running)
			Close();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(VAST_BIND_EVENT(OnWindowClose));

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
		{
			if (event.Handled)
				break;
			(*it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}	
	
	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
	}

	void Application::Close()
	{
		VAST_CORE_INFO("Application closed");
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		Close();
		return true;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			RendererCommand::Clear();

			RendererCommand::DrawIndexed(3);

			// ---- Draw GUI ------------------
			m_ImGuiLayer->Begin();
			
			for (Layer* layer : m_LayerStack)
				layer->OnGUIRender();
			
			m_ImGuiLayer->End();
			// --------------------------------

			m_Window->OnUpdate();
		}
	}

}