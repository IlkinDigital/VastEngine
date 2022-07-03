#include "ViewportPanel.h"

#include "AssetManager/SceneAsset.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Vast {

	ViewportPanel::ViewportPanel()
		: Panel("Viewport")
	{
	}

	ViewportPanel::ViewportPanel(const String& name)
		: Panel(name)
	{
	}

	void ViewportPanel::DrawPanel()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin(m_Name.c_str(), nullptr);

		m_Focused = ImGui::IsWindowFocused();
		m_Hovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents = !m_Focused || !m_Hovered;

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// Update width and height
		if (viewportPanelSize.x != m_Width || viewportPanelSize.y != m_Height)
		{
			m_Width = viewportPanelSize.x;
			m_Height = viewportPanelSize.y;
		}

		ImGui::Image((void*)m_ColorAttachment, ImVec2{ (float)m_Width, (float)m_Height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(SceneAsset::GetStaticTypeName()))
			{
				Asset* asset = (Asset*)payload->Data;
				if (m_DragDropFn)
					m_DragDropFn(asset->GetPath().string());
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		if (m_Gizmo)
			m_Gizmo->OnGUIRender();

		ImGui::End();
		ImGui::PopStyleVar();
	}

}