#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class FileDialog
	{
	public:
		static Filepath OpenFile(const char* filter);
		static Filepath SaveFile(const char* filter);
	};

}
