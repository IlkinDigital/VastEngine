#pragma once

#include "Math/MathCore.h"

#include <imgui.h>

namespace Vast {

	class WrapWidget
	{
	public:
		static void Begin(float padding = 5.0f, const Vector2& size = { 128.0f, 128.0f });
		static void End();
		
		/**
		* PushItem takes in a function that draws the item
		* @ func - void(const Vector2& size)
		*/
		template<typename Fn>
		static void PushItem(Fn func)
		{
			func(s_Size);
			ImGui::NextColumn();
		}
	private:
		static float s_Padding;
		static Vector2 s_Size;
	};
}
