#include "vastpch.h"
#include "AssetManager.h"

#include "Utils/FileIO/FileIO.h"

#include "Project/Project.h"
#include "Serialization/AssetSerializer.h"

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
		m_AssetMap.clear();
		IterateAndAddAssets(m_Project->GetContentFolderPath());
	}

	void AssetManager::IterateAndAddAssets(const Filepath& start)
	{
		for (auto& p : std::filesystem::directory_iterator(start))
		{
			if (p.is_directory())
				IterateAndAddAssets(p);
			else if (p.path().filename().extension() == ".asset")
			{
				Filepath path = FileIO::Relative(p.path(), m_Project->GetContentFolderPath());
				Ref<Asset> asset; 
				AssetSerializer as(m_Project, asset);
				as.Deserialize(path);
				asset = as.GetAsset();
				AddAsset(asset);
			}
		}
	}
}