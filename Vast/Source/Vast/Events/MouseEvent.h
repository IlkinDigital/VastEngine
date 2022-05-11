#pragma once

#include "Event.h"
#include "Vast/Input/MouseCodes.h"

namespace Vast {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_PosX(x), m_PosY(y) {}

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }

		String ToString() const override
		{
			StringStream ss;
			ss << "MouseMovedEvent: X(" << m_PosX << "), Y(" << m_PosY << ")";
			return ss.str();
		}

		EVENT_CATEGORY_IMPL((int)EventCategory::Input | (int)EventCategory::Mouse)
		EVENT_TYPE_IMPL(MouseMoved)
	private:
		float m_PosX, m_PosY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y)
			: m_OffsetX(x), m_OffsetY(y) {}

		float GetOffsetX() const { return m_OffsetX; }
		float GetOffsetY() const { return m_OffsetY; }

		String ToString() const override
		{
			StringStream ss;
			ss << "MouseScrolledEvent: Offset(" << m_OffsetX << ", " << m_OffsetY << ")";
			return ss.str();
		}

		EVENT_CATEGORY_IMPL((int)EventCategory::Input | (int)EventCategory::Mouse)
		EVENT_TYPE_IMPL(MouseScrolled)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		Mouse GetMouseButton() const { return m_MouseButton; }

		EVENT_CATEGORY_IMPL((int)EventCategory::Input | (int)EventCategory::Mouse | (int)EventCategory::MouseButton)
	protected:
		MouseButtonEvent(Mouse mouseButton)
			: m_MouseButton(mouseButton) {}

		Mouse m_MouseButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(Mouse mouseButton)
			: MouseButtonEvent(mouseButton) {}

		String ToString() const override
		{
			StringStream ss;
			ss << "MouseButtonPressedEvent: " << (int)m_MouseButton;
			return ss.str();
		}

		EVENT_TYPE_IMPL(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent: public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(Mouse mouseButton)
			: MouseButtonEvent(mouseButton) {}

		String ToString() const override
		{
			StringStream ss;
			ss << "MouseButtonReleasedEvent: " << (int)m_MouseButton;
			return ss.str();
		}

		EVENT_TYPE_IMPL(MouseButtonReleased)
	};

}