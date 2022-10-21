#pragma once

#include "GUI/Panels/Panel.h"

#include "Renderer/Texture2D.h"

namespace Vast {

	class DebugOutput
	{
	public:
		/*
		 * Appends window to subwindow stack,
		 * Don't call this function each frame
		 */ 
		static void Image(const Ref<Texture2D>& texture, const Vector2& uv0 = {0, 0}, const Vector2& uv1 = {1, 1});
	};

}
