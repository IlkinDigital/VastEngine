#pragma once

#include "Event.h"

namespace Vast {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: m_Width(width), m_Height(height) {}

		EVENT_CATEGORY_IMPL(EventCategory::Application)
		EVENT_TYPE_IMPL(WindowResize)
	private:
		uint32 m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CATEGORY_IMPL(EventCategory::Application)
		EVENT_TYPE_IMPL(WindowClose)
	};

}