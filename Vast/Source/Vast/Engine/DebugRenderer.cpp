#include "vastpch.h"
#include "DebugRenderer.h"

#include "Renderer/Renderer2D.h"

#include "Scene/Scene.h"

namespace Vast {

	void DebugRenderer::DrawLine(Scene& scene, const Vector3& start, const Vector3& end, const Vector4& color, float lifetime)
	{
		scene.m_DebugRenderer.m_DebugLines.emplace_back(DebugLine(start, end, color, lifetime));
	}

	void DebugRenderer::OnUpdate(Timestep ts)
	{
		for (int i = 0; i < m_DebugLines.size(); )
		{
			auto& item = m_DebugLines[i];

			item.OnUpdate(ts);
			if (item.IsAlive())
			{
				Renderer2D::DrawLine(item.Start, item.End, item.Color);
				i++;
			}
			else
				m_DebugLines.erase(m_DebugLines.cbegin() + i);
		}
	}

}