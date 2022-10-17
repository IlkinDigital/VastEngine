#pragma once

#include "Asset.h"
#include "AssetQueue.h"

#include "AssetTypes.h"

namespace Vast {

	class Project;

	class AssetManager
	{
	public:
		using AssetMap = std::unordered_map<Filepath, Ref<Asset>>;
	public:
		AssetManager() = default;

		void AddAsset(const Ref<Asset>& asset);
		void RemoveAsset(const Ref<Asset>& asset);

		Ref<Asset> GetAsset(const Filepath& path);
		bool ReplaceAsset(const Filepath& ref, const Ref<Asset>& asset);

		bool Exists(const Filepath& path) const;

		void Init();

		/*
		* Asset specific logic
		*/
		Ref<BoardSpriteAsset> FindAsset(const Ref<Board2D::Sprite>& sprite);
	private:
		void IterateAndAddAssets(const Filepath& start, AssetQueue& sceneAssets);
	private:
		AssetMap m_AssetMap; // Map of filepath to Asset
	};

}
