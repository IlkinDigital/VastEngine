#pragma once

#include "Asset.h"
#include "AssetQueue.h"

namespace Vast {

	class Project;

	class AssetManager
	{
	public:
		using AssetMap = std::unordered_map<Filepath, Ref<Asset>>;
	public:
		AssetManager() = default;
		AssetManager(const Ref<Project>& project);

		const Ref<Project>& GetProject() const { return m_Project; }
		void SetProject(const Ref<Project>& project) { m_Project = project; }

		void AddAsset(const Ref<Asset>& asset);
		void RemoveAsset(const Ref<Asset>& asset);

		Ref<Asset> GetAsset(const Filepath& path);

		void Init();

		static void Set(const Ref<AssetManager>& instance) { s_Instance = instance; }
		static const Ref<AssetManager>& Get() { return s_Instance; }
	private:
		void IterateAndAddAssets(const Filepath& start, AssetQueue& sceneAssets);
	private:
		static Ref<AssetManager> s_Instance;

		Ref<Project> m_Project;
		AssetMap m_AssetMap; // Map of filepath to Asset
	};

}
