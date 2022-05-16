#pragma once

#include "Vast/Renderer/Buffers/VertexBuffer.h"

namespace Vast {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32 size);
		OpenGLVertexBuffer(uint32 size);
		~OpenGLVertexBuffer() override;

		const BufferLayout& GetLayout() const override { return m_Layout; }
		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		void Bind() const override;
		void Unbind() const override;

		void SetVertexData(const void* data, uint32 size) override;

	private:
		RendererID m_RendererID = 0;
		BufferLayout m_Layout;
	};

}
