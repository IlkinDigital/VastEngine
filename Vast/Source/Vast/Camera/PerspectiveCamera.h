#pragma once

#include "Camera.h"
#include "Math/Math.h"

namespace Vast {
	
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar);

		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);

		const Mat4& GetView() const { return m_View; }
		const Mat4& GetViewProjection() const { return m_ViewProjection; }

		void SetPosition(const Vector3& position) { m_Position = position; CalculateView(); }
		void SetRotation(const Vector3& rotation) { m_Rotation = rotation; CalculateView(); }

		const Vector3& GetPosition() const { return m_Position; }
	private:
		void CalculateView();
	private:
		Mat4 m_View;
		Mat4 m_ViewProjection;

		Vector3 m_Position = { 0.0f, 0.0f, 0.0f };
		Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};
}
