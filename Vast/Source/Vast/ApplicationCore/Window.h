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
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual bool IsVSync() const = 0;
		virtual void SetVSync(bool flag) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props);
	};

}
