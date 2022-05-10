#include "Application.h"

namespace Vast {

	Application::Application(const String& name)
	{
		m_Window = Window::Create({ name });
	}

	Application::~Application()
	{
		Close();
	}

	void Application::Close()
	{
		VAST_CORE_INFO("Application closed");
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}