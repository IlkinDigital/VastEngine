#pragma once

#include "GUI/Panels/Panel.h"
#include "Gizmo/Gizmo3D.h"

namespace Vast {
	
	class ViewportPanel : public Panel
	{
	public:
		ViewportPanel();
		ViewportPanel(const String& name);

		template<typename Fn>
		void SetDragDropFn(Fn function) { m_DragDropFn = function; }

		void SetColorAttachment(const RendererID& attachment) { m_ColorAttachment = attachment; }
		void SetGizmo(const Ref<Gizmo3D>& gizmo) { m_Gizmo = gizmo; }

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }

		bool IsFocused() const { return m_Focused; }
		bool IsHovered() const { return m_Hovered; }
		
		virtual void DrawPanel() override;

	private:
		std::function<void(const String&)> m_DragDropFn = {};

		Ref<Gizmo3D> m_Gizmo = {};
		RendererID m_ColorAttachment = {};

		uint32 m_Width = 0;
		uint32 m_Height = 0;

		bool m_Focused = false;
		bool m_Hovered = false;
	};

}
