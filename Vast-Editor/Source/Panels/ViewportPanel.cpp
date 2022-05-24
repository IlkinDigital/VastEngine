#include "ViewportPanel.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Vast {

	void ViewportPanel::OnGUIRender(RendererID colorAttachment, Gizmo3D& gizmo)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);

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

		ImGui::Image((void*)colorAttachment, ImVec2{ (float)m_Width, (float)m_Height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ASSET"))
			{
				String path = (const char*)payload->Data;
				if (m_DragDropFn)
					m_DragDropFn(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		gizmo.OnGUIRender();

		ImGui::End();
		ImGui::PopStyleVar();
	}

}