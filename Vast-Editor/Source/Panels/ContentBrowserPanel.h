#pragma once

#include "Vast.h"

#include <filesystem>

namespace Vast {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnGUIRender();
	private:
		String m_RootDirectory = "Assets";
		std::filesystem::path m_CurrentPath = m_RootDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
