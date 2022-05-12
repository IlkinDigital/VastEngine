#include "vastpch.h"
#include "Application.h"

#include "Vast/Events/EventDispatcher.h"

namespace Vast {

	Application::Application(const String& name)
	{
		m_Window = Window::Create({ name });
		m_Window->SetEventCallback(VAST_BIND_EVENT(OnEvent));
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

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
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
		return false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

}