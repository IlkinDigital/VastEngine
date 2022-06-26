#pragma once

#include "Scene/Scene.h"
#include "Panel.h"

namespace Vast {

	class LineupPanel : public Panel
	{
	public:
		LineupPanel() = default;
		LineupPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context) { m_Context = context; }

		virtual void DrawPanel() override;
	private:
		void DrawNode(Entity entity);
	private:
		Ref<Scene> m_Context;
	};

}