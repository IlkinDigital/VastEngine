#include "SpriteSheetEditor.h"

#include "EditorLayout/Layout.h"

#include <imgui_internal.h>

namespace Vast {

	void SpriteSheetView::DrawPanel()
	{
	}

	void SpriteSheetSettings::DrawPanel()
	{
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