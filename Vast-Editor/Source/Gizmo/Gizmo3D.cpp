#include "Gizmo3D.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Vast {

	static int ConvertGizmoType(Gizmo3D::GizmoType type)
	{
		switch (type)
		{
		case Gizmo3D::GizmoType::Translation:
			return ImGuizmo::OPERATION::TRANSLATE;
		case Gizmo3D::GizmoType::Rotation:
			return ImGuizmo::OPERATION::ROTATE;
		case Gizmo3D::GizmoType::Scale:
			return ImGuizmo::OPERATION::SCALE;
		}

		return -1;
	}

	void Gizmo3D::OnGUIRender()
	{
		if (m_SelectedEntity.IsValid() && m_Type != GizmoType::None)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			Mat4 transform = tc.Transform();

			Vector3 snapValues = { 0.01f, 0.01f, 0.01f };

			ImGuizmo::Manipulate(Math::ValuePointer(*m_CameraView), Math::ValuePointer(*m_CameraProjection),
				(ImGuizmo::OPERATION)ConvertGizmoType(m_Type), ImGuizmo::LOCAL, Math::MValuePointer(transform),
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
	}

	void Gizmo3D::UpdateData(Entity selectedEntity, const Mat4& cameraView, const Mat4& cameraProjection)
	{
		m_SelectedEntity = selectedEntity;
		m_CameraView = &cameraView;
		m_CameraProjection = &cameraProjection;
	}

}