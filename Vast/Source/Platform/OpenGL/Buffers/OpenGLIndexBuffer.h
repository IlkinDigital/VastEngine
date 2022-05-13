#pragma once

#include "Vast/Renderer/Buffers/IndexBuffer.h"

namespace Vast {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);
		~OpenGLIndexBuffer() override;

		uint32 GetIndexCount() const override { return m_IndexCount; }

		void Bind() const override;
		void Unbind() const override;
	private:
		RendererID m_RendererID;
		uint32 m_IndexCount;
	};

}
