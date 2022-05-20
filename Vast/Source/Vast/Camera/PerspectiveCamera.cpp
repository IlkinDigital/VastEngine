#include "vastpch.h"
#include "PerspectiveCamera.h"

#include "Math/MathOperations.h"

namespace Vast {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar)
	{
		SetProjection(fov, aspectRatio, zNear, zFar);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
	{
		m_Projection = Math::Perspective(fov, aspectRatio, zNear, zFar);
		m_ViewProjection = m_Projection * m_View;
	}

	void PerspectiveCamera::CalculateView()
	{
		Mat4 rotation = Math::Rotate(Mat4(1.0f), Math::Radians(m_Rotation.x), { 1.0f, 0.0f, 0.0f })
			* Math::Rotate(Mat4(1.0f), Math::Radians(m_Rotation.y), { 0.0f, 1.0f, 0.0f })
			* Math::Rotate(Mat4(1.0f), Math::Radians(m_Rotation.z), { 0.0f, 0.0f, 1.0f });

		Mat4 transform = Math::Translate(Mat4(1.0f), m_Position) *
			rotation;

		m_View = Math::Inverse(transform);
		m_ViewProjection = m_Projection * m_View;
	}

}