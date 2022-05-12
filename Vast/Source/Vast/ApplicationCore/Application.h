#pragma once

#include "Vast/Core/CoreMinimal.h"

#include "Window.h"
#include "Vast/Events/ApplicationEvent.h"

#include "Vast/ApplicationCore/Layer/LayerStack.h"

namespace Vast {

	class Application
	{
	public:
		Application(const String& name);
		virtual ~Application();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		void Run();
		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& event);
	private:
		Scope<Window> m_Window;
		String m_Name;

		LayerStack m_LayerStack;
		bool m_Running = true;
	};

	Application* CreateApplication();
}
