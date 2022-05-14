#pragma once

#include "Renderer/VertexArray.h"

namespace Vast {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;

		void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		const DArray<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

		void Bind() const override;
		void Unbind() const override;

	private:
		DArray<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		RendererID m_RendererID;
	};

}
