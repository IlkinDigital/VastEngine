#include "ViewportPanel.h"

#include <imgui.h>

namespace Vast {

	void ViewportPanel::OnGUIRender(RendererID colorAttachment)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");

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

		ImGui::End();
		ImGui::PopStyleVar();
	}

}