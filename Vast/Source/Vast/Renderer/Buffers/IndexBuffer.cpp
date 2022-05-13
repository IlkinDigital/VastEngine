#include "vastpch.h"
#include "IndexBuffer.h"

#include "Platform/OpenGL/Buffers/OpenGLIndexBuffer.h"

namespace Vast {

	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}

}