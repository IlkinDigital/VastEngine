#include "vastpch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Vast {

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32 size)
	{
		glCreateBuffers(1, &m_RendererID);
		VAST_ASSERT(m_RendererID, "Vertex buffer couldn't be created");
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetVertexData(const void* data, uint32 size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

}