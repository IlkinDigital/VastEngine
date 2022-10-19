#pragma once

#include "GUI/Panels/Panel.h"

#include "Renderer/Texture2D.h"
#include "Project/Project.h"

namespace Vast {

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();

		virtual void DrawPanel() override;
	private:
		void RenameAsset(const Filepath& path, const String& newName);
	private:
		Filepath m_CurrentPath;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;

		bool m_DialogOpen = false;
		String m_CurrentName;
		Filepath m_RenamePath;
	};

}
