#pragma once

#include "Scene/Entity.h"

namespace Vast {

	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;

		void OnGUIRender(Entity entity);
		
	private:
		void DrawComponents(Entity entity);
	private:
		Entity m_Entity = {};
	};

}
