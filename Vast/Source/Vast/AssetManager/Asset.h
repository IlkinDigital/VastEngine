#pragma once

#include "AssetCore.h"

#include "UUID.h"

namespace Vast {
	
	class Asset
	{
	public:
		Asset(AssetType type, const Filepath& source, const Filepath& target);
		Asset(AssetType type, const Filepath& filepath);
	private:
		UUID m_UUID;
		AssetType m_Type;
		Filepath m_Path; // Relative to the Assets directory
	};

}
