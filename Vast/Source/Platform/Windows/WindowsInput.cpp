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

	Vector2 Input::GetMousePosition()
	{
		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		double x = 0.0f, y = 0.0f;
		glfwGetCursorPos(window, &x, &y);

		return { x, y };
	}
}

#endif