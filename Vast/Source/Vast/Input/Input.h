#pragma once

#include "Math/Math.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Vast {

	class Input
	{
	public:
		static bool IsPressed(Key key);
		static bool IsPressed(Mouse mouseButton);

		static Vector2 GetMousePosition();
	};

}