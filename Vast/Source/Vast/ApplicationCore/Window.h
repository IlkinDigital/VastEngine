#pragma once

#include "Vast/Core/CoreMinimal.h"

namespace Vast {

	struct WindowProps
	{
		String Title;
		uint32 Width, Height;

		WindowProps(const String& title, uint32 width = 1920, uint32 height = 1080)
			: Title(title), Width(width), Height(height) 
		{
			VAST_CORE_INFO("'{0}' window initialized: W({1}), H({2})", Title, Width, Height);
		}
	};

	class Window
	{
	public:
		static Scope<Window> Create(const WindowProps& props);
	};

}
