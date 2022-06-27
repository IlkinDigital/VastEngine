#pragma once

#include "Scene/Entity.h"

namespace Vast {

	class EditorLayout
	{
	public:
		/**
		* Sets up docking space
		* Must be called OnGUIRender
		*/
		static void BeginDockspace(const char* title);
		static void EndDockspace();

		static Entity GetSelectedEntity() { return s_SelectedEntity; }
		static void SetSelectedEntity(Entity entity) { s_SelectedEntity = entity; }
	private:
		static Entity s_SelectedEntity;
	};

}
