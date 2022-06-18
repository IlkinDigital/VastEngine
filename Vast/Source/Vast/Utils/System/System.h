#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class System
	{
	public:
		static void RunCommand(const Filepath& directory, const String& command);
	};

}