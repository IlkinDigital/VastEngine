#include "vastpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLInterface.h"

namespace Vast {

	Ref<RendererInterface> RendererCommand::s_Interface = CreateRef<OpenGLInterface>();

}