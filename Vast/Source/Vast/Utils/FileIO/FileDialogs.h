#pragma once

#include "CoreMinimal.h"

namespace Vast::FileIO {

	class Dialogs
	{
	public:
		static Filepath OpenFile(const char* filter);
		static Filepath SaveFile(const char* filter);
	};

}
