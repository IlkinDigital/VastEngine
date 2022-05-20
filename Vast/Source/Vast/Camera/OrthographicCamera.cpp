#include "vastpch.h"
#include "OrthographicCamera.h"

namespace Vast {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_View(1.0f), m_ViewProjection(1.0f), m_Position(0.0f), m_Rotation(0.0f)
	{
		SetProjection(left, right, bottom, top);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Projection = Math::OrthographicMatrix(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjection = m_Projection * m_View;
	}

	void OrthographicCamera::CalculateView()
	{
		Mat4 transform = Math::Translate(Mat4(1.0f), m_Position) *
						 Math::Rotate(Mat4(1.0f), m_Rotation, Vector3(0.0f, 0.0f, 1.0f));

		m_View = Math::Inverse(transform);
		m_ViewProjection = m_Projection * m_View;
	}

}