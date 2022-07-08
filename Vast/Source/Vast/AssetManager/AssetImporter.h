#pragma once

#include "AssetTypes.h"
#include "Project/Project.h"

namespace Vast {

	class AssetImporter
	{
	public:
		AssetImporter(const Ref<Project>& project)
			: m_Project(project) {}

		Ref<Texture2DAsset> ImportTexture(const Filepath& imagePath, const Filepath& toPath);
		void ExportTexture(const Ref<Texture2DAsset>& asset, const Filepath& toPath);
	private:
		Ref<Project> m_Project;
	};

}