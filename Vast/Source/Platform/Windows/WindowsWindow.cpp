#include "WindowsWindow.h"

namespace Vast {

	static bool s_GLFWInitialized = false;

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		// Initialize GLFW
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			VAST_ASSERT(success, "GLFW couldn't intialize");
			s_GLFWInitialized = success;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		VAST_INFO("Created a {0} window: W({1}), H({2})", m_Data.Title, m_Data.Width, m_Data.Height);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// TODO: Set GLFW Callbacks
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool flag)
	{
		if (flag)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSyncFlag = flag;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}


}