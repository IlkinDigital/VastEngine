#pragma once

#include "Vast/Renderer/GraphicsContext.h"

// TODO: make platform independent
struct GLFWwindow;

namespace Vast {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}
