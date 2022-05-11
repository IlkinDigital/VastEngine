#pragma once

#include "Event.h"

namespace Vast {

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename Ty, typename Fn>
		bool Dispatch(const Fn& function)
		{
			if (m_Event.GetEventType() == Ty::GetStaticType())
			{
				m_Event.Handled |= function((Ty&)m_Event);
				return true;
			}

			return false;
		}
	private:
		Event& m_Event;
	};

}