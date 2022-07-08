#include "vastpch.h"
#include "Texture2DAsset.h"

namespace Vast {

	Texture2DAsset::Texture2DAsset(const String& name, const Filepath& path, UUID uuid)
		: Asset(AssetType::Texture2D, name, path, uuid)
	{
	}

	Ref<Asset> Texture2DAsset::Clone() const
	{
		auto ta = CreateRef<Texture2DAsset>(m_Name, m_Path, m_UUID);
		ta->SetFileData(m_FileData);
		ta->SetTexture(m_Texture);
		return ta;
	}

}