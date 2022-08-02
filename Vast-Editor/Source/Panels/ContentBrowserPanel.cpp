#include "ContentBrowserPanel.h"

#include "GUI/FontManager.h"

#include "AssetManager/AssetTypes.h"
#include "AssetManager/AssetImporter.h"
#include "Serialization/AssetSerializer.h"
#include "Utils/FileIO/FileIO.h"
#include "Utils/FileIO/FileDialogs.h"

#include "EditorLayer.h"
#include <imgui.h>
#include <optick.h>

namespace Vast {

	static const char* ToAssetName(AssetType type)
	{
		switch (type)
		{
		case AssetType::Texture2D:
			return "Texture";
		case AssetType::BoardSprite:
			return "Board Sprite";
		case AssetType::BoardSpriteSheet:
			return "Board Sprite Sheet";
		case AssetType::BoardFlipbook:
			return "Board Flipbook";
		case AssetType::Scene:
			return "Scene";
		}

		return "None";
	}

	void ContentBrowserPanel::RenameAsset(const Filepath& path, const String& newName)
	{
		OPTICK_EVENT();

		auto& assetManager = m_Project->GetAssetManager();
		auto asset = assetManager->GetAsset(path);

		if (asset->GetType() != AssetType::None)
		{
			asset->SetName(m_CurrentName);
			asset->SetPath(path.parent_path() / newName);
		}

		Filepath absolute = m_Project->GetContentFolderPath();
		absolute += (path.string() + ".asset");
		std::filesystem::remove(absolute);

		AssetSerializer as(m_Project, asset);
		as.Serialize();
		assetManager->ReplaceAsset(path, asset);
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: Panel("Content Browser")
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
		m_FrameIcon = Texture2D::Create("Resources/Icons/TestFrame.png");
	}

	ContentBrowserPanel::ContentBrowserPanel(const Ref<Project>& project)
		: Panel("Content Browser"), m_Project(project)
	{
		m_CurrentPath = m_Project->GetContentFolderPath();
		m_FolderIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
		m_FrameIcon = Texture2D::Create("Resources/Icons/TestFrame.png");
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
			Ref<Texture2DAsset> asset = importer.ImportTexture(origin, rel);
			m_Project->GetAssetManager()->AddAsset(asset);
		}

		static float padding = 5.0f;
		static float thumbnailSize = 155.0f;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = panelWidth / (thumbnailSize + padding);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { ImGui::GetStyle().FramePadding.x, 100 });
		int assetButtonID = 567;
		for (auto& p : std::filesystem::directory_iterator(m_CurrentPath))
		{
			String path = p.path().string();
			String filename = p.path().stem().filename().string();
			if (!m_DialogOpen)
				m_CurrentName = p.path().filename().replace_extension("").string();

			Ref<Asset> asset = CreateRef<Asset>(AssetType::None, "None", "None", 0);
			auto assetManager = m_Project->GetAssetManager();

			bool drawName = false;

			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3, 0.3, 0.3, 0.2 });
			if (p.is_directory())
			{
				ImGui::ImageButton((ImTextureID)m_FolderIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					m_CurrentPath /= filename;	
				}

				drawName = true;
				ImGui::Text(filename.c_str());
				ImGui::NextColumn();
			}
			else if (p.path().extension() == ".asset")
			{
				asset = assetManager->GetAsset(
					FileIO::Relative(p.path(), m_Project->GetContentFolderPath()).replace_extension("")
				);

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
				
				ImGui::PushID(path.c_str());

				ImGui::BeginChild(assetButtonID++, { thumbnailSize, thumbnailSize + 100 }, true);
				ImVec2 buttonStart = ImGui::GetCursorPos();

				Filepath relative = FileIO::Relative(p.path(), m_Project->GetContentFolderPath());
				relative.replace_extension("");
				Ref<Asset> asset = m_Project->GetAssetManager()->GetAsset(relative);
				
				if (asset->GetType() == AssetType::Texture2D)
				{
					Ref<Texture2D> tex = RefCast<Texture2DAsset>(m_Project->GetAssetManager()->GetAsset(relative))->GetTexture();
					float width = thumbnailSize;
					float height = tex->GetHeight() * (thumbnailSize / tex->GetWidth());
					ImGui::Image((ImTextureID)tex->GetRendererID(), { width, height }, { 0, 1 }, { 1, 0 });
				}
				else
				{
					ImGui::Image((ImTextureID)m_FileIcon->GetRendererID(), 
						{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				}


				// Frame
				auto framePos = ImGui::GetCursorScreenPos();
				ImGui::Dummy({20, 5});
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(framePos, ImVec2(framePos.x + thumbnailSize, framePos.y + 100), IM_COL32(75, 75, 75, 255), 0.1f);

				drawName = true;

				ImGui::Dummy({ 1, 0 });
				ImGui::SameLine();
				ImGui::Text(filename.c_str());

				ImGui::Dummy({ 20, 40 });

				ImGui::PushFont(FontManager::GetFont(FontManager::WeightType::Bold));
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 90));

				ImGui::Dummy({ 1, 0 });
				ImGui::SameLine();
				ImGui::Text(ToAssetName(asset->GetType()));

				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.0f, 0.0f, 0.2f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.2f });
				ImGui::SetCursorPos(buttonStart);
				
				ImGui::Button("##btn", { thumbnailSize, thumbnailSize + 100 });
				
				ImGui::PopStyleColor(3);

				// If .asset file is double clicked
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					switch (asset->GetType())
					{
					case AssetType::BoardFlipbook:
						EditorLayer::Get()->OpenFlipbookEditor(RefCast<BoardFlipbookAsset>(asset));
						break;
					}
				}

				// Drag and Drop payload
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::SetDragDropPayload(asset->GetTypeName(), asset.get(), sizeof(*asset.get()));
					ImGui::EndDragDropSource();
				}
				
				ImGui::EndChild();
				ImGui::PopStyleVar(2);
				ImGui::PopID();

				ImGui::Dummy({ 0, padding * 3.0f });
				ImGui::NextColumn();
			}
			
			ImGui::PopStyleColor(2);

			bool openRenameDialog = false;

			if (ImGui::BeginPopupContextItem(filename.c_str()))
			{
				if (ImGui::MenuItem("Rename"))
				{
					openRenameDialog = true;
					m_RenamePath = FileIO::Relative(p.path(), m_Project->GetContentFolderPath()).replace_extension("");
					
				}
				if (ImGui::MenuItem("Delete"))
				{
					if (asset->GetType() != AssetType::None)
						assetManager->RemoveAsset(asset);

					std::filesystem::remove(p.path());
				}
				if (asset->GetType() == AssetType::Texture2D && ImGui::MenuItem("Create Sprite Sheet"))
				{
					AssetImporter ai(m_Project);
					Filepath ss = asset->GetPath();
					ss.replace_filename("SpriteSheetYay");
					m_Project->GetAssetManager()->AddAsset(ai.CreateSpriteSheet(RefCast<Texture2DAsset>(asset), ss));
				}


				ImGui::EndPopup();
			}
			
			if (openRenameDialog)
			{
				ImGui::OpenPopup("RenameDialog");
				m_DialogOpen = true;
			}


		}

		//ImGui::PopStyleVar(1);

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("RenameDialog", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoMove))
		{
			char buffer[64];
			strcpy(buffer, m_CurrentName.c_str());
			if (ImGui::InputText("##Rename", buffer, sizeof(buffer)))
				m_CurrentName = buffer;

			if (ImGui::Button("Rename"))
			{
				Filepath original = m_Project->GetContentFolderPath();
				original += m_RenamePath;

				if (std::filesystem::is_directory(original)) // Folder renaming
				{
					Filepath newPath = original;
					newPath.replace_filename(m_CurrentName);

					if (!std::filesystem::exists(newPath))
					{
						std::filesystem::rename(original, newPath);

						ImGui::CloseCurrentPopup();
						m_DialogOpen = false;
					}
				}
				else // File renaming
				{
					Filepath newPath = m_RenamePath;
					newPath.replace_filename(m_CurrentName);
					if (!m_Project->GetAssetManager()->Exists(newPath) || FileIO::Normalize(m_RenamePath) == FileIO::Normalize(newPath))
					{
						RenameAsset(m_RenamePath, m_CurrentName);

						ImGui::CloseCurrentPopup();
						m_DialogOpen = false;
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				m_DialogOpen = false;
			}

			ImGui::EndPopup();
		}

		ImGui::Columns(1);

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Board2D"))
			{
				if (ImGui::MenuItem("Flipbook"))
				{
					Filepath path = FileIO::Relative(m_CurrentPath / "NewFlipbook.asset", m_Project->GetContentFolderPath());
					Ref<BoardFlipbookAsset> bfa = CreateRef<BoardFlipbookAsset>("NewFlipbook", path, UUID());
					bfa->SetFlipbook(CreateRef<Board2D::Flipbook>());
					AssetSerializer as(m_Project, bfa);
					as.Serialize();
					AssetManager::Get()->Init();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("New Folder"))
			{
				std::filesystem::create_directory(m_CurrentPath / "NewFolder");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

}