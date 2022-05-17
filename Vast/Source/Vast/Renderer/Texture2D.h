#pragma once

#include "Texture.h"

namespace Vast {

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32 width, uint32 height);
		static Ref<Texture2D> Create(const String& filepath);
	};

}