#pragma once

#include "Asset.h"

namespace Vast {

	class AssetManager
	{
	public:
		using AssetMap = std::unordered_map<String, Asset>;
	public:
		AssetManager(const String& rootDir, const AssetMap& assetMap)
			: m_RootDirectory(rootDir), m_AssetMap(assetMap) {}
	public:
		String m_RootDirectory = "Assets";
		AssetMap m_AssetMap; // Map of filepath to Asset
	};

}
