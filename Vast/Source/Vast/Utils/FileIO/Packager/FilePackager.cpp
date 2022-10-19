#include "vastpch.h"
#include "FilePackager.h"

namespace Vast {

	String FilePackager::Pack(const Package& package)
	{
		OPTICK_EVENT();

		String string;
		string.reserve(package.First.size() + package.Second.size() + 1);

		string.append(package.First);
		string.push_back('\0');
		string.append(package.Second);

		return string;
	}

	Package FilePackager::Unpack(const String& pack)
	{
		OPTICK_EVENT();

		auto index = pack.find('\0');
		if (index == pack.npos)
		{
			VAST_CORE_ERROR("Cannot unpack - \"{0}\", data is not packed", pack);
			return {};
		}

		String first = pack.substr(0, index);
		String second = pack.substr(index + 1, pack.size() - index - 1);

		return { first, second };
	}

}