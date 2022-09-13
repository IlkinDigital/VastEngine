#pragma once

#include "AssetTypes.h"
#include "Project/Project.h"

namespace Vast {

	class AssetImporter
	{
	public:
		AssetImporter(const Ref<Project>& project)
			: m_Project(project) {}

		Ref<BoardSpriteSheetAsset> CreateSpriteSheet(const Ref<Texture2DAsset>& texture, const Filepath& toPath);
		Ref<BoardSpriteAsset> CreateSprite(const Ref<BoardSpriteSheetAsset>& sheet, uint16 col, uint16 row, const Filepath& toPath);

		Ref<Texture2DAsset> ImportTexture(const Filepath& imagePath, const Filepath& toPath);
		void ExportTexture(const Ref<Texture2DAsset>& asset, const Filepath& toPath);
	private:
		Ref<Project> m_Project;
	};

}