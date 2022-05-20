#include "vastpch.h"
#include "SceneCamera.h"

namespace Vast {

	SceneCamera::SceneCamera()
	{
		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32 width, uint32 height)
	{
		VAST_ASSERT(height != 0, "Division by zero, height must be greates than 0");
		m_AspectRatio = (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = Math::Perspective(m_PerspProps.VerticalFOV, m_AspectRatio, m_PerspProps.NearClip, m_PerspProps.FarClip);
		}
		else
		{
			float left = -m_OrthoProps.Size * m_AspectRatio * 0.5f;
			float right = m_OrthoProps.Size * m_AspectRatio * 0.5f;
			float bottom = -m_OrthoProps.Size * 0.5f;
			float top = m_OrthoProps.Size * 0.5f;

			m_Projection = Math::OrthographicMatrix(left, right, bottom, top, m_OrthoProps.NearClip, m_OrthoProps.FarClip);
		}
	}

}