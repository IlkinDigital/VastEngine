#pragma once

#include "Scene/Entity.h"

#include "Panels/Panel.h"

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

		static bool BeginWindow(const Subwindow& subwindow, bool* open);
		static void EndWindow();
	};

}
