#pragma once

#include "Asset.h"
#include "Project/Project.h"

namespace Vast {

	class AssetManager
	{
	public:
		using AssetMap = std::unordered_map<Filepath, Ref<Asset>>;
	public:
		AssetManager(const Ref<Project>& project);
	public:
		Ref<Project> m_Project;
		AssetMap m_AssetMap; // Map of filepath to Asset
	};

}
