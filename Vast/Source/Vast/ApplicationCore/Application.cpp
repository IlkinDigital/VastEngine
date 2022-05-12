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
			m_Window->OnUpdate();
		}
	}

}