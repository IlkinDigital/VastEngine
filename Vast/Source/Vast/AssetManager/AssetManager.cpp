#include "vastpch.h"
#include "AssetManager.h"

#include "Utils/FileIO/FileIO.h"

#include "Project/Project.h"
#include "Serialization/AssetSerializer.h"

#include "Clock/Clock.h"

namespace Vast {

	Ref<AssetManager> AssetManager::s_Instance = CreateRef<AssetManager>();

	AssetManager::AssetManager(const Ref<Project>& project)
		: m_Project(project)
	{
		Init();
	}

	void AssetManager::AddAsset(const Ref<Asset>& asset)
	{
		const auto& path = asset->GetPath();
		if (path.has_extension() && path.extension() == ".asset" && m_AssetMap.find(path.stem()) == m_AssetMap.end())
			m_AssetMap.insert({ path.stem(), asset});
	}

	void AssetManager::RemoveAsset(const Ref<Asset>& asset)
	{
	}

	Ref<Asset> AssetManager::GetAsset(const Filepath& path)
	{
		return m_AssetMap[path.stem()];
	}

	void AssetManager::Init()
	{
		OPTICK_EVENT();

		m_AssetMap.clear();

		/// Scenes must be deserialized last because they may reference
		/// not yet created assets
		DArray<Ref<Asset>> sceneAssets;

		IterateAndAddAssets(m_Project->GetContentFolderPath(), sceneAssets);

		for (const auto& asset : sceneAssets)
		{
			AssetSerializer as(m_Project, asset);
			as.Deserialize(asset->GetPath());
			AddAsset(as.GetAsset());
		}
	}

	void AssetManager::IterateAndAddAssets(const Filepath& start, DArray<Ref<Asset>>& sceneAssets)
	{
		for (auto& p : std::filesystem::directory_iterator(start))
		{
			if (p.is_directory())
			{
				IterateAndAddAssets(p, sceneAssets);
			}
			else if (p.path().filename().extension() == ".asset")
			{
				OPTICK_EVENT("OnExtension  == .asset");

				Filepath path = FileIO::Relative(p.path(), m_Project->GetContentFolderPath());
				Ref<Asset> asset; 
				AssetSerializer as(m_Project, asset);
				
				// TODO: This check requires asset to be deserialized twice, fix it

				if (as.SerializationType(path) == AssetType::Scene)
				{
					sceneAssets.push_back(as.GetAsset());
					continue;
				}

				as.Deserialize(path);
				asset = as.GetAsset();
				AddAsset(asset);

				VAST_CORE_TRACE("{0}", asset->GetName());
			}
		}
	}
}