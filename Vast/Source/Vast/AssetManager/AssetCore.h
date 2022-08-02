#pragma once

#include "CoreMinimal.h"

namespace Vast {

	enum class AssetType : uint16
	{
		None = 0, Texture2D, BoardSpriteSheet, BoardSprite, BoardFlipbook, Scene
	};

}