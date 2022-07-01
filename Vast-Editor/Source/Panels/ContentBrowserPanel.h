#pragma once

#include "Panel.h"

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
		Ref<Project> m_Project;
		Filepath m_CurrentPath;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
