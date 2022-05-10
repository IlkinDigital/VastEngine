#pragma once

#include "Vast/Core/CoreMinimal.h"

#include "Window.h"

namespace Vast {

	class Application
	{
	public:
		Application(const String& name);
		virtual ~Application();

		void Run();

		void Close();
	private:
		Scope<Window> m_Window;
		String m_Name;
		
		bool m_Running = true;
	};

	Application* CreateApplication();
}
