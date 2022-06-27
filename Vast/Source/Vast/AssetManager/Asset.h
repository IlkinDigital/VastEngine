#pragma once

#include "AssetCore.h"

#include "UUID.h"

namespace Vast {
	
	class Asset
	{
	public:
		Asset(const Filepath& source, const Filepath& target, UUID uuid);

		virtual void Serialize(const Filepath& path) = 0;
		virtual void Deserialize(const Filepath& path) = 0;
	protected:
		UUID m_UUID;
		String m_Name;
		Filepath m_Path; // Relative to the Content folder
	};

}
