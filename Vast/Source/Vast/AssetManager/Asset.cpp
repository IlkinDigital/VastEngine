#include "vastpch.h"
#include "Asset.h"

namespace Vast {

	Asset::Asset(const Filepath& source, const Filepath& target, UUID uuid)
		: m_UUID(uuid)
	{
		if (source != target)
		{
			if (std::filesystem::copy_file(source, target))
				m_Path = target;
			else
				VAST_CORE_ASSERT(false, "Couldn't import a file");
		}
	}

}