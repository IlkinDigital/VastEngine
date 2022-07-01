#pragma once

#include "TextureAsset.h"
#include "Project/Project.h"

namespace Vast {

	class AssetImporter
	{
	public:
		AssetImporter(const Ref<Project>& project)
			: m_Project(project) {}

		Ref<TextureAsset> ImportTexture(const Filepath& imagePath, const Filepath& toPath);
		void ExportTexture(const Ref<TextureAsset>& asset, const Filepath& toPath);
	private:
		Ref<Project> m_Project;
	};

}