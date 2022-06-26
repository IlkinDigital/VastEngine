#include "ContentBrowserPanel.h"

#include "GUI/FontManager.h"

#include <imgui.h>

namespace Vast {

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
	}

	ContentBrowserPanel::ContentBrowserPanel(const Filepath& rootDir)
	{
		m_RootDirectory = rootDir;
		m_CurrentPath = m_RootDirectory;
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
	}

	void ContentBrowserPanel::SetRootDirectory(const Filepath& rootDir)
	{
		m_RootDirectory = rootDir;
		m_CurrentPath = m_RootDirectory;
	}

	void ContentBrowserPanel::DrawPanel()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentPath != m_RootDirectory)
		{
			ImGui::PushFont(FontManager::GetFont(FontManager::WeightType::Bold));
			if (ImGui::Button("<--"))
				m_CurrentPath = m_CurrentPath.parent_path();
			ImGui::PopFont();
		}

		static float padding = 10.0f;
		static float thumbnailSize = 128.0f;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = panelWidth / (thumbnailSize + padding);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& p : std::filesystem::directory_iterator(m_CurrentPath))
		{
			String path = p.path().string();
			String filename = p.path().filename().string();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3, 0.3, 0.3, 1 });
			if (p.is_directory())
			{
				ImGui::ImageButton((ImTextureID)m_FolderIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					m_CurrentPath /= filename;
				}
			}
			else
			{
				ImGui::PushID(path.c_str());
				ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropSource())
				{
					const char* itemPath = path.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ASSET", itemPath, (path.size() + 1) * sizeof(char));
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();
			}
			ImGui::PopStyleColor(2);

			ImGui::Text(filename.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

}