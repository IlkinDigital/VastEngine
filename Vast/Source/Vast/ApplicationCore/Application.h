#pragma once

#include "CoreMinimal.h"

#include "Window.h"
#include "Events/ApplicationEvent.h"

#include "ApplicationCore/Layer/LayerStack.h"
#include "GUI/ImGuiLayer.h"

namespace Vast {

	class Application
	{
	public:
		Application(const String& name);
		virtual ~Application();

		static Application& Get() { return *s_Instance; }

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }

		void Run();
		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		Scope<Window> m_Window;
		String m_Name;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;
		bool m_Running = true;

		float m_LastFrameTime;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
