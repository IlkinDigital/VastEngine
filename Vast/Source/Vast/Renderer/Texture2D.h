#pragma once

#include "Texture.h"

namespace Vast {

	class Texture2D : public Texture
	{
	public:
		// TODO: Create asset manager
		virtual const String& GetFilepath() const = 0;

		static Ref<Texture2D> Create(uint32 width, uint32 height);
		static Ref<Texture2D> Create(const String& filepath);
	};

}