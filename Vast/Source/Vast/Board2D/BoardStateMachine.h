#pragma once

#include "BoardFlipbook.h"

namespace Vast::Board2D {

	/**
	 * State machine that maps state
	 * to certain texture
	 */

	class StateMachine
	{
	public:
		StateMachine();

		const Ref<Flipbook>& GetCurrentFlipbook() const { return m_OutputFlipbook; }
		void PushFlipbook(uint16 state, const Ref<Flipbook>& flipbook);

		void ActivateState(uint16 state);
		void EditState(uint16 state, const Ref<Flipbook>& flipbook);
	private:
		std::unordered_map<uint16, Ref<Flipbook>> m_StateFlipbookMap; // State to flibook map

		uint16 m_CurrentState = -1;
		Ref<Flipbook> m_OutputFlipbook; // Current Board2D::Flipbook
	};

}
