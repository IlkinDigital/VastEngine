#include "SpriteSheetEditor.h"

#include "GUI/EditorLayout/Layout.h"
#include "DebugWindow.h"

#include <imgui_internal.h>

namespace Vast {

	void SpriteSheetView::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		const auto& sheet = m_SpriteSheet->GetSheet();
		const Vector2& stride = m_SpriteSheet->GetStride();
		const Vector2& size = { m_SpriteSheet->GetSheet()->GetWidth(), m_SpriteSheet->GetSheet()->GetHeight() };
		ImVec2 displaySize = ImGui::GetContentRegionAvail();
		ImVec2 displayStart = ImGui::GetCursorScreenPos();

		ImGui::Image((ImTextureID)sheet->GetRendererID(), displaySize, { 0, 1 }, { 1, 0 });

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		float pt0X = displayStart.x;
		for (int i = 0; i < size.x / stride.x; i++)
		{
			drawList->AddLine({ pt0X, displayStart.y }, { pt0X, displayStart.y + displaySize.y }, 0x0f3f9fff);
		}

		ImGui::End();
	}

	void SpriteSheetSettings::DrawPanel()
	{
		ImGui::Begin(m_Name.c_str());

		auto stride = m_SpriteSheet->GetSpriteSheet()->GetStride();

		float strideX = stride.x;
		float strideY = stride.y;

		if (ImGui::DragFloat("Stride X", &strideX))
			m_SpriteSheet->GetSpriteSheet()->SetStride({ strideX, strideY });
		if (ImGui::DragFloat("Stride Y", &strideY))
			m_SpriteSheet->GetSpriteSheet()->SetStride({ strideX, strideY });

		if (ImGui::Button("Show sprite"))
		{
			m_Extracted = m_SpriteSheet->GetSpriteSheet()->ExtractSprite(0, 0);
			DebugOutput::Image(m_Extracted->GetTexture(), m_Extracted->GetUVCoords()[0], m_Extracted->GetUVCoords()[1]);
		}

		ImGui::End();
	}

	/**
	* Sprite Sheet Editor
	*/

	SpriteSheetEditor::SpriteSheetEditor()
		: Subwindow("Sprite Sheet Editor")
	{
	}

	void SpriteSheetEditor::OnAttach()
	{
		m_View.Open();
		m_Settings.Open();
	}

	void SpriteSheetEditor::DrawPanel()
	{
		if (!EditorLayout::BeginWindow(*this, &m_IsOpen))
		{
			EditorLayout::EndWindow();
			return;
		}

		// DockBuilder 
		if (!m_InitializedDockspace)
		{
			ImGui::SetWindowSize({ m_DefaultSize.x, m_DefaultSize.y });

			ImGuiID dockspaceID = GetUUID();

			ImGui::DockBuilderRemoveNode(dockspaceID);
			ImGui::DockBuilderAddNode(dockspaceID,
				ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
			ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

			ImGuiID leftID = -1;
			auto rightID = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Right, 0.3f, nullptr, &leftID);

			ImGui::DockBuilderDockWindow(m_Settings.GetName().c_str(), rightID);
			ImGui::DockBuilderDockWindow(m_View.GetName().c_str(), leftID);

			ImGui::DockBuilderFinish(dockspaceID);
			m_InitializedDockspace = true;
		}

		m_View.OnGUIRender();
		m_Settings.OnGUIRender();

		EditorLayout::EndWindow();
	}

}