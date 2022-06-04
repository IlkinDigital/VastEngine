#pragma once

#include "Scene/Entity.h"
#include "NativeScripting/ScriptBuffer.h"

namespace Vast {

	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;

		void OnGUIRender(Entity entity, const ScriptBuffer& scripts);
		
	private:
		void DrawComponents(Entity entity, const ScriptBuffer& scripts);
	private:
		Entity m_Entity = {};
	};

}
