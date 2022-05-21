#pragma once

#include "Vast.h" 

namespace Vast {

	class Gizmo3D
	{
	public:
		enum class GizmoType { None = 0, Translation, Rotation, Scale };
	public:
		Gizmo3D() = default;

		void SetGizmoType(GizmoType type) { m_Type = type; }

		void UpdateData(Entity selectedEntity, const Mat4& cameraView, const Mat4& cameraProjection);

		void OnGUIRender();
	private:
		Entity m_SelectedEntity;
		const Mat4* m_CameraView;
		const Mat4* m_CameraProjection;
		GizmoType m_Type = GizmoType::None;
	};

}
