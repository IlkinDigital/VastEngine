#pragma once

#include "CoreMinimal.h"

namespace Vast::FileIO {

	class Dialogs
	{
	public:
		static String OpenFile(const char* filter);
		static String SaveFile(const char* filter);
	};

}
