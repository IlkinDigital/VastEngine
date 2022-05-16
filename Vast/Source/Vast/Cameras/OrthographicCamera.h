#pragma once

#include "Math/MathOperations.h"

namespace Vast {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const Mat4& GetView() const { return m_View; }
		const Mat4& GetProjection() const { return m_Projection; }
		const Mat4& GetViewProjection() const { return m_ViewProjection; }

		void SetPosition(const Vector3& position) { m_Position = position; CalculateView(); }
		void SetRotation(float rotation) { m_Rotation = rotation; CalculateView(); }

		const Vector3& GetPosition() const { return m_Position; }
	private:
		void CalculateView();
	private:
		Mat4 m_View;
		Mat4 m_Projection;
		Mat4 m_ViewProjection;

		Vector3 m_Position;
		float m_Rotation;
	};

}
