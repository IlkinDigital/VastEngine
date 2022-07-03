#include "ContentBrowserPanel.h"

#include "GUI/FontManager.h"

#include "AssetManager/Texture2DAsset.h"
#include "Utils/FileIO/FileIO.h"
#include "Utils/FileIO/FileDialogs.h"

#include <imgui.h>
#include <Vast/AssetManager/AssetImporter.h>

namespace Vast {

	ContentBrowserPanel::ContentBrowserPanel()
		: Panel("Content Browser")
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
	}

	ContentBrowserPanel::ContentBrowserPanel(const Ref<Project>& project)
		: Panel("Content Browser"), m_Project(project)
	{
		m_CurrentPath = m_Project->GetContentFolderPath();
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
	}

	void ContentBrowserPanel::DrawPanel()
	{
		if (!m_Project)
			return;

		ImGui::Begin(m_Name.c_str());

		if (m_CurrentPath != m_Project->GetContentFolderPath())
		{
			ImGui::PushFont(FontManager::GetFont(FontManager::WeightType::Bold));
			if (ImGui::Button("<--"))
				m_CurrentPath = m_CurrentPath.parent_path();
			ImGui::PopFont();
		}
		ImGui::SameLine();
		if (ImGui::Button("Import Texture"))
		{
			Filepath origin = FileDialog::OpenFile("");
			Filepath rel = FileIO::Relative(m_CurrentPath, m_Project->GetContentFolderPath());
			rel /= (origin.filename().stem().string() + ".asset");

			VAST_TRACE("Origin path: {0}", origin.string());
			VAST_TRACE("New path: {0}", rel.string());

			AssetImporter importer(m_Project);
			Ref<Asset> asset = importer.ImportTexture(origin, rel);
			const auto& am = m_Project->GetAssetManager();
			am->Init();
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
			String filename = p.path().stem().filename().string();

			bool drawName = false;

			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3, 0.3, 0.3, 1 });
			if (p.is_directory())
			{
				ImGui::ImageButton((ImTextureID)m_FolderIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					m_CurrentPath /= filename;
				}

				drawName = true;
			}
			else if (p.path().extension() == ".asset")
			{
				ImGui::PushID(path.c_str());
				Filepath relative = FileIO::Relative(p.path(), m_Project->GetContentFolderPath());
				Ref<Asset> asset = m_Project->GetAssetManager()->GetAsset(relative);
				
				if (asset->GetType() == AssetType::Texture2D)
				{
					Ref<Texture2D> tex = RefCast<Texture2DAsset>(m_Project->GetAssetManager()->GetAsset(relative))->GetTexture();
					float tot = tex->GetHeight() + tex->GetWidth();
					ImGui::ImageButton((ImTextureID)tex->GetRendererID(),
						{ ((float)tex->GetWidth() * 2 / tot) * thumbnailSize, ((float)tex->GetHeight() * 2 / tot) * thumbnailSize }, { 0, 1 }, { 1, 0 });
				}
				else
				{
					ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), 
						{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload(asset->GetTypeName(), asset.get(), sizeof(*asset.get()));
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();

				drawName = true;
			}
			ImGui::PopStyleColor(2);

			if (drawName)
			{
				ImGui::Text(filename.c_str());
				ImGui::NextColumn();
			}
		}

		ImGui::Columns(1);

		ImGui::End();
	}

}