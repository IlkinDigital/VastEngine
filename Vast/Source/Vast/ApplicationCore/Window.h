#pragma once

#include "CoreMinimal.h"
#include "Vast/Events/Event.h"

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
		using EventCallback = std::function<void(Event&)>;
	public:
		virtual ~Window() = default;

		/**
		 * Called for required intitializations
		 * Primarily used to allow sharing of window handles between projects
		 */ 
		virtual bool Init() { return true; }

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual bool IsVSync() const = 0;
		virtual void SetVSync(bool flag) = 0;

		virtual void SetEventCallback(const EventCallback& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Ref<Window> Create(const WindowProps& props);
	};

}
