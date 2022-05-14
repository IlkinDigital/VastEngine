#include "vastpch.h"

#ifdef VAST_PLATFORM_WINDOWS

#include "Input/Input.h"
#include "ApplicationCore/Application.h"

#include <GLFW/glfw3.h>

namespace Vast {

	bool Input::IsPressed(Key key)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		return GLFW_PRESS == glfwGetKey(window, (int)key);
	}
	
	bool Input::IsPressed(Mouse mouseButton)
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		return GLFW_PRESS == glfwGetMouseButton(window, (int)mouseButton);
	}
}

#endif