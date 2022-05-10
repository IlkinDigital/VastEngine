#pragma once

#include "Vast/ApplicationCore/Window.h"

#include <GLFW/glfw3.h>

namespace Vast {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;

		uint32 GetWidth() const override { return m_Data.Width; }
		uint32 GetHeight() const override { return m_Data.Height; }

		bool IsVSync() const override { return m_Data.VSyncFlag; }
		void SetVSync(bool flag) override;

		void* GetNativeWindow() const override;
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			String Title;
			uint32 Width, Height;
			bool VSyncFlag;
		};
		WindowData m_Data;
	};

}
