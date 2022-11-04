#pragma once

#include "Scene/Entity.h"
#include "GUI/Panels/Panel.h"


namespace Vast {

	class LineupPanel : public Panel
	{
	public:
		LineupPanel();
		LineupPanel(const Ref<Scene>& context);

		const Ref<Entity>& GetSelectedEntity() const { return m_SelectedEntity; }
		void DeselectEntity() { *m_SelectedEntity = Entity(); }
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