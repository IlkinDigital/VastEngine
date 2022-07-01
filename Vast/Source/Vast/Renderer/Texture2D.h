#pragma once

#include "Texture.h"
#include "AssetManager/TextureAsset.h"

namespace Vast {

	class Texture2D : public Texture
	{
	public:
		// TODO: Create asset manager
		virtual const Filepath& GetFilepath() const = 0;

		static Ref<Texture2D> Create(uint32 width, uint32 height);
		// DEPRECATED: Asset manager must be used
		static Ref<Texture2D> Create(const Filepath& filepath);
		static Ref<Texture2D> Create(const Ref<TextureAsset>& asset);
	};

}