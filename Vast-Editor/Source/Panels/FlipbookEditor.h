#pragma once

#include "Panel.h"

#include "Board2D/BoardFlipbook.h"

namespace Vast {

	class FlipbookEditor : public Panel
	{
	public:
		void SetContext(const Ref<Board2D::Flipbook>& flipbook) { m_Context = flipbook; }

		virtual void DrawPanel() override;
	private:
		Ref<Board2D::Flipbook> m_Context;
	};

}
