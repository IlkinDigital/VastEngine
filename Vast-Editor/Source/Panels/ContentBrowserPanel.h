#pragma once

#include "Renderer/Texture2D.h"
#include "Panel.h"

namespace Vast {

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(); // LEGACY: Takes a path relative to Editor
		ContentBrowserPanel(const Filepath& rootDir);

		void SetRootDirectory(const Filepath& rootDir);

		virtual void DrawPanel() override;
	private:
		Filepath m_RootDirectory = "Assets";
		Filepath m_CurrentPath = m_RootDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
