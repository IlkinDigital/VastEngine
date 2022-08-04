#include "vastpch.h"
#include "Asset.h"

namespace Vast {

	Asset::Asset(AssetType assetType, const String& name, const Filepath& path, UUID uuid)
		: m_AssetType(assetType), m_Name(name), m_Path(path), m_UUID(uuid)
	{
		m_Path.replace_extension("");
	}

	Ref<Asset> Asset::Clone() const
	{
		return CreateRef<Asset>(m_AssetType, m_Name, m_Path, m_UUID);
	}

}