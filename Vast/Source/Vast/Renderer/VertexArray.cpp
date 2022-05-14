#include "vastpch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vast {

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}
}