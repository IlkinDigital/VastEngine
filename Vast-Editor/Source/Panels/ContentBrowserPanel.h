#pragma once

#include "GUI/Panels/Panel.h"

#include "Renderer/Texture2D.h"
#include "Project/Project.h"

namespace Vast {

	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(); // DEPRECATED: Takes a path relative to Editor
		ContentBrowserPanel(const Ref<Project>& project);

		void SetProject(const Ref<Project>& project) { m_Project = project; m_CurrentPath = m_Project->GetContentFolderPath(); }

		virtual void DrawPanel() override;
	private:
		void RenameAsset(const Filepath& path, const String& newName);
	private:
		Ref<Project> m_Project;
		Filepath m_CurrentPath;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_FrameIcon;

		bool m_DialogOpen = false;
		String m_CurrentName;
		Filepath m_RenamePath;
	};

}
