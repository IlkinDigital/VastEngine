#include "vastpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Vast {

	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:  
		case ShaderDataType::Float2: 
		case ShaderDataType::Float3: 
		case ShaderDataType::Float4: 
		case ShaderDataType::Mat3:   
		case ShaderDataType::Mat4:   return GL_FLOAT;
		case ShaderDataType::Int:	 
		case ShaderDataType::Int2:	 
		case ShaderDataType::Int3:   
		case ShaderDataType::Int4:   return GL_INT;
		}

		VAST_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: m_RendererID(0)
	{
		glCreateVertexArrays(1, &m_RendererID);
		VAST_CORE_ASSERT(m_RendererID, "Vertex array couldn't be created");
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		Bind();
		buffer->Bind();

		const BufferLayout& layout = buffer->GetLayout();

		for (uint32 i = 0; i < layout.GetElements().size(); i++)
		{
			auto& element = layout.GetElements()[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				CalculateComponentCount(element.Type),
				ShaderDataTypeToOpenGL(element.Type),
				element.Normalized,
				layout.GetStride(),
				(const void*)element.Offset
			);
		}

		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		Bind();
		buffer->Bind();

		m_IndexBuffer = buffer;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}