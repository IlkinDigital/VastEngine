#pragma once

#include "Board2D/BoardFlipbook.h"

namespace Vast {

	struct SpriteComponent
	{
		Ref<Board2D::Flipbook> Flipbook;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const Ref<Board2D::Flipbook>& flipbook)
			: Flipbook(flipbook) {}
	};

}