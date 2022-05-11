#pragma once

#include "Event.h"
#include "Vast/Input/KeyCodes.h"

namespace Vast {

	class KeyEvent : public Event
	{
	public:
		Key GetKeyCode() const { return m_KeyCode; }

		EVENT_CATEGORY_IMPL((int)EventCategory::Keyboard | (int)EventCategory::Input)
	protected:
		KeyEvent(Key keyCode)
			: m_KeyCode(keyCode) {}

		Key m_KeyCode;
	}; 

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key keyCode, uint16 repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		uint16 GetRepeatCount() const { return m_RepeatCount; }

		String ToString() const override
		{
			StringStream ss;
			ss << "KeyPressedEvent: " << (int)m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_TYPE_IMPL(KeyPressed)
	private:
		uint16 m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key keyCode)
			: KeyEvent(keyCode) {}

		String ToString() const override
		{
			StringStream ss;
			ss << "KeyReleasedEvent: " << (int)m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE_IMPL(KeyReleased)
	};
}