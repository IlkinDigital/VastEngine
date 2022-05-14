#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Vast {

	class Input
	{
	public:
		static bool IsPressed(Key key);
		static bool IsPressed(Mouse mouseButton);
	};

}