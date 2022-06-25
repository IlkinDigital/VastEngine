#include "vastpch.h"
#include "FileIO.h"

namespace Vast {

    Filepath FileIO::Relative(const Filepath& path, const Filepath& relativeTo)
    {
		Filepath relative;
		auto starts = path.string().find(relativeTo.string());
		if (starts < path.string().size())
		{
			String pathStr = path.string();
			uint32 count = relativeTo.string().size();
			relative = pathStr.erase(starts, count);
		}

		if (relative.empty())
		{
			VAST_CORE_WARN("Failed to make '{0}' relative to '{1}', absolute path returned", path.string(), relativeTo.string());
			return path;
		}

		return relative;
    }

}