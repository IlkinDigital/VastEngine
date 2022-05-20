#pragma once

#include "Vast.h"

namespace Vast {

	class LineupPanel
	{
	public:
		LineupPanel() = default;
		LineupPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context) { m_Context = context; }

		Entity GetSelected() const { return m_SelectedNode; }

		void OnGUIRender();
	private:
		void DrawNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectedNode = {};
	};

}