#pragma once

#include "CoreMinimal.h"

namespace Vast {

	enum class EventCategory
	{
		None = 0,
		Application = VAST_BIT(0),
		Input		= VAST_BIT(1),
		Keyboard    = VAST_BIT(2),
		Mouse		= VAST_BIT(3),
		MouseButton = VAST_BIT(4)
	};

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowUnfocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};


	class Event
	{
	public:
		bool Handled = false;
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual String ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & (int)category;
		}
	};

#define EVENT_TYPE_IMPL(type) static EventType GetStaticType() { return EventType::type; }\
							  EventType GetEventType() const override { return GetStaticType(); }\
							  const char* GetName() const override { return #type; }

#define EVENT_CATEGORY_IMPL(category) int GetCategoryFlags() const override { return (int)category; }
}