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
		
		void OnGUIRender(RendererID colorAttachment);

	private:
		uint32 m_Width = 0;
		uint32 m_Height = 0;

		bool m_Focused = false;
		bool m_Hovered = false;
	};

}
