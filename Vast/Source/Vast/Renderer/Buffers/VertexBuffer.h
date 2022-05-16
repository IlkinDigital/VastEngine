#pragma once

#include "BufferLayout.h"

namespace Vast {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetVertexData(const void* data, uint32 size) = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32 size);
		static Ref<VertexBuffer> Create(uint32 size);
	};

}