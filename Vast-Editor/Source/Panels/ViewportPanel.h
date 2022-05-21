#pragma once

#include "Vast.h"

namespace Vast {
	
	class ViewportPanel
	{
	public:
		ViewportPanel() = default;

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }

		bool IsFocused() const { return m_Focused; }
		bool IsHovered() const { return m_Hovered; }

		void SetGizmoType(int type) { m_GizmoType = type; }
		
		void OnGUIRender(RendererID colorAttachment, Entity selectedEntity, const Mat4& cameraView, const Mat4& cameraProjection);

	private:
		uint32 m_Width = 0;
		uint32 m_Height = 0;

		bool m_Focused = false;
		bool m_Hovered = false;

		int m_GizmoType = -1;
	};

}
