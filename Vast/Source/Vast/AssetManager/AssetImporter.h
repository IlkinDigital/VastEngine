#pragma once

#include "AssetTypes.h"
#include "Project/Project.h"

namespace Vast {

	class AssetImporter
	{
	public:
		AssetImporter() = default;

		Ref<BoardSpriteSheetAsset> CreateSpriteSheet(const Ref<Texture2DAsset>& texture, const Filepath& toPath);
		Ref<BoardSpriteAsset> CreateSprite(const Ref<BoardSpriteSheetAsset>& sheet, uint16 col, uint16 row, const Filepath& toPath);

		Ref<Texture2DAsset> ImportTexture(const Filepath& imagePath, const Filepath& toPath);
		void ExportTexture(const Ref<Texture2DAsset>& asset, const Filepath& toPath);
	};

}