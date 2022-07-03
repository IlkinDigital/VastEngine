#include "vastpch.h"
#include "FilePackager.h"

namespace Vast {

	String FilePackager::Pack(const Package& package)
	{
		String string;
		string.reserve(package.First.size() + package.Second.size() + 1);

		string.append(package.First);
		string.push_back('\0');
		string.append(package.Second);

		return string;
	}

	Package FilePackager::Unpack(const String& pack)
	{
		auto index = pack.find('\0');
		if (index == pack.npos)
		{
			VAST_CORE_ERROR("Cannot unpack, data is not packed");
			return {};
		}

		return { pack.substr(0, index), pack.substr(index + 1, pack.size() - index - 1) };
	}

}