#pragma once

#include "Vast.h"

#include <filesystem>

namespace Vast {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(); // LEGACY: Takes a path relative to Editor
		ContentBrowserPanel(const Filepath& rootDir);

		void SetRootDirectory(const Filepath& rootDir);

		void OnGUIRender();
	private:
		Filepath m_RootDirectory = "Assets";
		Filepath m_CurrentPath = m_RootDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
