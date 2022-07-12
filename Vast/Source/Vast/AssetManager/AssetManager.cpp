#include "vastpch.h"
#include "AssetManager.h"

#include "AssetQueue.h"

#include "Project/Project.h"
#include "Serialization/AssetSerializer.h"

#include "Utils/FileIO/FileIO.h"
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
		Filepath path = FileIO::Normalize(asset->GetPath());
		if (m_AssetMap.find(path) == m_AssetMap.end())
			m_AssetMap.insert({ path, asset});
	}

	void AssetManager::RemoveAsset(const Ref<Asset>& asset)
	{
	}

	Ref<Asset> AssetManager::GetAsset(const Filepath& path)
	{
		Filepath normPath = FileIO::Normalize(path);
		if (m_AssetMap.find(normPath) == m_AssetMap.end())
			return CreateRef<Asset>();

		return m_AssetMap.at(normPath);
	}

	bool AssetManager::Exists(const Filepath& path) const
	{
		return m_AssetMap.find(FileIO::Normalize(path)) != m_AssetMap.end();
	}

	void AssetManager::Init()
	{
		OPTICK_EVENT();

		m_AssetMap.clear();

		/// Assets must be deserialized in order because they may reference
		/// not yet created assets
		AssetQueue assetQueue;

		IterateAndAddAssets(m_Project->GetContentFolderPath(), assetQueue);

		{
			OPTICK_EVENT("AddAssetsToRegistry");

			for (const auto& asset : assetQueue)
			{
				AssetSerializer as(m_Project, asset);
				as.Deserialize(asset->GetPath());
				AddAsset(as.GetAsset());
			}
		}
	}

	void AssetManager::IterateAndAddAssets(const Filepath& start, AssetQueue& queue)
	{
		for (auto& p : std::filesystem::directory_iterator(start))
		{
			if (p.is_directory())
			{
				IterateAndAddAssets(p, queue);
			}
			else if (p.path().filename().extension() == ".asset")
			{
				OPTICK_EVENT("ReadHeadAndEnqueueAsset");

				Filepath path = FileIO::Relative(p.path(), m_Project->GetContentFolderPath());
				Ref<Asset> asset; 
				AssetSerializer as(m_Project, asset);

				if (as.SerializationType(path) != AssetType::None)
				{
					queue.Enqueue(as.GetAsset());
				}

				VAST_CORE_TRACE("{0}", as.GetAsset()->GetName());
			}
		}
	}
}