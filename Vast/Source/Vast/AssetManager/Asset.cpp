#include "vastpch.h"
#include "Asset.h"

namespace Vast {

	Asset::Asset(AssetType assetType, const String& name, const Filepath& path, UUID uuid)
		: m_AssetType(assetType), m_Name(name), m_Path(path), m_UUID(uuid)
	{
	}

}