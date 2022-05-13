#pragma once

#include "Renderer/RendererCore.h"

namespace Vast {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual uint32 GetIndexCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<IndexBuffer> Create(uint32* indices, uint32 count);
	};

}

