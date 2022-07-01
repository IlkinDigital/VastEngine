#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class FileIO
	{
	public:
		// Returns filepath relative to specified 'relativeTo' parameter
		static Filepath Relative(const Filepath& path, const Filepath& relativeTo);
		static bool IsImage(const Filepath& path);
	};

}
