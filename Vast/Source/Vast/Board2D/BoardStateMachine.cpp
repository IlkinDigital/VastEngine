#include "vastpch.h"
#include "BoardStateMachine.h"

namespace Vast::Board2D {

	StateMachine::StateMachine(const Ref<Flipbook>& outFlipbook)
		: m_OutputFlipbook(outFlipbook)
	{
	}

	void StateMachine::PushFlipbook(uint16 state, const Ref<Flipbook>& flipbook)
	{
		if (m_StateFlipbookMap.find(state) != m_StateFlipbookMap.end())
		{
			VAST_CORE_ERROR("Specified state already exists");
			return;
		}

		m_StateFlipbookMap.insert({ state, flipbook });
	}

	void StateMachine::ActivateState(uint16 state)
	{
		if (m_StateFlipbookMap.find(state) == m_StateFlipbookMap.end())
		{
			VAST_CORE_ERROR("Specified state doesn't exist");
			return;
		}

		if (m_CurrentState != state)
		{
			m_CurrentState = state;
			m_OutputFlipbook = m_StateFlipbookMap[state];
		}
	}

}