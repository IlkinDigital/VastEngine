#include "vastpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLInterface.h"

namespace Vast {

	RendererInterface* RendererCommand::s_Interface = new OpenGLInterface();

}