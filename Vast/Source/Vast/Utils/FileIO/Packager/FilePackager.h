#pragma once

#include "CoreMinimal.h"

namespace Vast {

	struct Package
	{
		String First;
		String Second;
	};

	class FilePackager
	{
	public:
		static String Pack(const Package& package);
		static Package Unpack(const String& pack);
	};

}