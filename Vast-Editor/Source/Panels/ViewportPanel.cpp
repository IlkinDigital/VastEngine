#include "ViewportPanel.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Vast {

	void ViewportPanel::OnGUIRender(RendererID colorAttachment, Entity selectedEntity, const Mat4& cameraView, const Mat4& cameraProjection)
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

		// Gizmos
		if (selectedEntity.IsValid() && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
		
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			Mat4 transform = tc.Transform();

			Vector3 snapValues = { 0.01f, 0.01f, 0.01f };

			ImGuizmo::Manipulate(Math::ValuePointer(cameraView), Math::ValuePointer(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, Math::MValuePointer(transform),
				nullptr, Math::MValuePointer(snapValues));

			if (ImGuizmo::IsUsing())
			{
				Vector3 translation{}, rotation{}, scale{};
				ImGuizmo::DecomposeMatrixToComponents(Math::ValuePointer(transform), Math::MValuePointer(translation), Math::MValuePointer(rotation), Math::MValuePointer(scale));

				tc.Translation = translation;
				tc.Rotation = Math::Radians(rotation);
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

}