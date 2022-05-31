#include "vastpch.h"
#include "Application.h"

#include "Events/EventDispatcher.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/Renderer2D.h"

#include "Input/Input.h"

#include "Clock/Clock.h"

#include <GLFW/glfw3.h>

namespace Vast {

	Application* Application::s_Instance = nullptr;


	Application::Application(const String& name)
		: m_LastFrameTime(Clock::EpochSeconds())
	{
		VAST_ASSERT(!s_Instance, "Application already exists");

		s_Instance = this;

		m_Window = Window::Create({ name });
		m_Window->SetEventCallback(VAST_BIND_EVENT(OnEvent));
		
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Init renderer
		RendererCommand::Init();
		Renderer2D::Init();
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
		dispatcher.Dispatch<WindowResizeEvent>(VAST_BIND_EVENT(OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		RendererCommand::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// ---- Setup timestep ---------------
			float epoch = Clock::EpochSeconds();
			float ts = epoch - m_LastFrameTime;
			m_LastFrameTime = epoch;
			// -----------------------------------

			RendererCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
			RendererCommand::Clear();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(Timestep(ts));

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