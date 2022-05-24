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

		void SetRotationSnap(float value) { m_SnapValues[1] = value; }
		void SetTrScSnap(float value) { m_SnapValues[0] = value; m_SnapValues[2] = value; }

		const Vector3& GetSnapValues() const { return m_SnapValues; }

		void UpdateData(Entity selectedEntity, const Mat4& cameraView, const Mat4& cameraProjection);

		void OnGUIRender();
	private:
		Entity m_SelectedEntity = {};
		const Mat4* m_CameraView = nullptr;
		const Mat4* m_CameraProjection = nullptr;

		// Snapping value of rotation, translation, scale
		Vector3 m_SnapValues = { 0.01f, 0.01f, 0.01f }; 
		
		GizmoType m_Type = GizmoType::None;
	};

}
