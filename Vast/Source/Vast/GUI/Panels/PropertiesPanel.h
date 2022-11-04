#pragma once

#include "GUI/Panels/Panel.h"
#include "Scene/Entity.h"

namespace Vast {

	class PropertiesPanel : public Panel
	{
	public:
		PropertiesPanel()
			: Panel("Properties") {}
		PropertiesPanel(const String& name)
			: Panel(name) {}

		void SetContextEntity(const Ref<Entity>& entity) { m_Entity = entity; }

		virtual void DrawPanel() override;
	private:
		void DrawComponents(Entity entity);
	private:
		Ref<Entity> m_Entity = {};
	};

}
