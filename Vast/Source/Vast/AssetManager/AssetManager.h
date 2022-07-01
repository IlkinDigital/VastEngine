#pragma once

#include "Asset.h"

namespace Vast {

	class Project;

	class AssetManager
	{
	public:
		using AssetMap = std::unordered_map<Filepath, Ref<Asset>>;
	public:
		AssetManager() = default;
		AssetManager(const Ref<Project>& project);

		void SetProject(const Ref<Project>& project) { m_Project = project; }

		void AddAsset(const Ref<Asset>& asset);
		void RemoveAsset(const Ref<Asset>& asset);

		Ref<Asset> GetAsset(const Filepath& path);
		
		void Init();
	private:
		void IterateAndAddAssets(const Filepath& start);
	public:
		Ref<Project> m_Project;
		AssetMap m_AssetMap; // Map of filepath to Asset
	};

}
