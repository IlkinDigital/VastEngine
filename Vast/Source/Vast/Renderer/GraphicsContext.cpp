#include "vastpch.h"
#include "GraphicsContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Vast {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        return CreateScope<OpenGLContext>((GLFWwindow*)window);
    }

}