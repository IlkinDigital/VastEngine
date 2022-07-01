#include "vastpch.h"
#include "TextureAsset.h"

namespace Vast {

	Texture2DAsset::Texture2DAsset(const String& name, const Filepath& path, UUID uuid)
		: Asset(AssetType::Texture, name, path, uuid)
	{
	}

}