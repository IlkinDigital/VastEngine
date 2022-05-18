#include "vastpch.h"
#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Vast {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
	{
		return CreateRef<OpenGLFramebuffer>(specification);
	}

}