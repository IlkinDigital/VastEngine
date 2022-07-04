#include "vastpch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Vast {

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count)
		: m_IndexCount(count), m_RendererID(0)
	{
		OPTICK_EVENT();

		glCreateBuffers(1, &m_RendererID);
		VAST_ASSERT(m_RendererID, "Index buffer couldn't be created");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}