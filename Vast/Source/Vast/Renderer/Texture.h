#pragma once

#include "RendererCore.h"

namespace Vast {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual RendererID GetRendererID() const = 0;

		virtual void SetData(void* data, uint32 size) = 0;
		virtual void Bind(uint32 slot = 0) const = 0;
	};
}