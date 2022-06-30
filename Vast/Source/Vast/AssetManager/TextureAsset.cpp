#include "vastpch.h"
#include "TextureAsset.h"

namespace Vast {

	TextureAsset::TextureAsset(const String& name, const Filepath& path, UUID uuid)
		: Asset(AssetType::Texture, name, path, uuid)
	{
	}

}