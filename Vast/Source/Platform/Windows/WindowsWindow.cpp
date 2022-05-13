#include "vastpch.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

#include "Vast/Events/ApplicationEvent.h"
#include "Vast/Events/KeyboardEvent.h"
#include "Vast/Events/MouseEvent.h"

namespace Vast {

	static bool s_GLFWInitialized = false;

#ifdef VAST_PLATFORM_WINDOWS
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
#endif

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
			VAST_CORE_ASSERT(success, "GLFW couldn't intialize");
			s_GLFWInitialized = success;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		VAST_INFO("Created a {0} window: W({1}), H({2})", m_Data.Title, m_Data.Width, m_Data.Height);

		m_Context = GraphicsContext::Create((void*)m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		
		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->Width = width;
				data->Height = height;

				WindowResizeEvent event(width, height);
				data->Callback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				
				WindowCloseEvent event;
				data->Callback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

				// TODO: Create adequate keycode conversion (Currently using glfw's key codes)
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event((Key)key, 0);
						data->Callback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event((Key)key, 1);
						data->Callback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event((Key)key);
						data->Callback(event);
						break;
					}
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event((Mouse)button);
						data->Callback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event((Mouse)button);
						data->Callback(event);
						break;
					}
				}
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xpos, (float)ypos);
				data->Callback(event);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data->Callback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);

		// TODO: glfwTerminate
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
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