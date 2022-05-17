#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class FileIO
	{
	public:
		static String GetFileName(const String& filepath);
		static String GetFileExtension(const String& filepath);
	};

}
