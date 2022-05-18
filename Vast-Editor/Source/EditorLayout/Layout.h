#pragma once

namespace Vast {

	class EditorLayout
	{
	public:
		/**
		* Sets up docking space
		* Must be called OnGUIRender
		*/
		static void BeginDockspace();
		static void EndDockspace();
	};

}
