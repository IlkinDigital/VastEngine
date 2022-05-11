#pragma once

#include "Vast/Core/CoreMinimal.h"

#include "Window.h"
#include "Vast/Events/ApplicationEvent.h"


namespace Vast {

	class Application
	{
	public:
		Application(const String& name);
		virtual ~Application();

		void OnEvent(Event& event);

		void Run();
		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& event);
	private:
		Scope<Window> m_Window;
		String m_Name;
		
		bool m_Running = true;
	};

	Application* CreateApplication();
}
