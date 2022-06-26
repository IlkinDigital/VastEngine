#pragma once

#include "Panel.h"
#include "Scene/Entity.h"

namespace Vast {

	class PropertiesPanel : public Panel
	{
	public:
		PropertiesPanel() = default;

		virtual void DrawPanel() override;
		
	private:
		void DrawComponents(Entity entity);
	private:
		Entity m_Entity = {};
	};

}
