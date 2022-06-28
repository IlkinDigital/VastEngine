#pragma once

#include "Scene/Scene.h"
#include "Panel.h"

namespace Vast {

	class LineupPanel : public Panel
	{
	public:
		LineupPanel();
		LineupPanel(const Ref<Scene>& context);

		const Ref<Entity>& GetSelectedEntity() const { return m_SelectedEntity; }
		void SetContext(const Ref<Scene>& context) { m_Context = context; }

		void Init();

		virtual void DrawPanel() override;
	private:
		void DrawNode(Entity entity);
	private:
		Ref<Entity> m_SelectedEntity;
		Ref<Scene> m_Context;
	};

}