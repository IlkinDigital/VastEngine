#include "vastpch.h"
#include "VertexBuffer.h"

#include "Platform/OpenGL/Buffers/OpenGLVertexBuffer.h"

namespace Vast {
 
    Ref<VertexBuffer> Vast::VertexBuffer::Create(float* vertices, uint32 size)
    {
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }
}