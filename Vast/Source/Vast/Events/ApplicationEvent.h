#pragma once

#include "Event.h"

namespace Vast {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height)
			: m_Width(width), m_Height(height) {}

		uint32 GetWidth() const  { return m_Width;  }
		uint32 GetHeight() const { return m_Height; }

		String ToString() const override
		{
			StringStream ss;
			ss << "WindowResizeEvent: W(" << m_Width << "), H(" << m_Height << ")";
			return ss.str();
		}

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