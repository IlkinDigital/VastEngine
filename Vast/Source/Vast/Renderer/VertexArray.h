#pragma once

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"

namespace Vast {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

		virtual const DArray<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<VertexArray> Create();
	};

}

