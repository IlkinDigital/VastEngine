#include "vastpch.h"
#include "Asset.h"

namespace Vast {

	Asset::Asset(AssetType type, const Filepath& source, const Filepath& target)
	{
		if (std::filesystem::copy_file(source, target))
		{
			m_Type = type;
			m_Path = target;
		}
		else
			VAST_CORE_ASSERT(false, "Couldn't import a file");
	}

	Asset::Asset(AssetType type, const Filepath& filepath)
		: m_Type(type), m_Path(filepath)
	{
		VAST_CORE_ASSERT(std::filesystem::exists(filepath), "File doesn't exist");
	}

}