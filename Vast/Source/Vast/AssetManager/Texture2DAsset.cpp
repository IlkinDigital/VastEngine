#include "vastpch.h"
#include "Texture2DAsset.h"

namespace Vast {

	Texture2DAsset::Texture2DAsset(const String& name, const Filepath& path, UUID uuid)
		: Asset(AssetType::Texture2D, name, path, uuid)
	{
	}

}