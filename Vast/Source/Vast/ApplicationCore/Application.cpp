#include "vastpch.h"
#include "Application.h"

#include "Vast/Events/EventDispatcher.h"

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

		float vertices[3 * 3]
		{
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f
		};

		m_VertexBuffer = VertexBuffer::Create(vertices, 3 * 3 * sizeof(float));

		GLuint indices[3]
		{
			0, 1, 2
		};

		m_IndexBuffer = IndexBuffer::Create(indices, 3);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		const String vertSrc = R"(
			#version 430 core

			layout (location = 0) in vec4 a_Pos;

			void main()
			{
				gl_Position = a_Pos;
			}
		)";

		const String fragSrc = R"(
			#version 430 core
			
			layout (location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.2, 0.4, 0.8, 1.0);
			}

		)";

		m_Shader = Shader::Create("BasicShader", vertSrc, fragSrc);
		m_Shader->Bind();
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

			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

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