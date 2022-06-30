#pragma once

#include "AssetCore.h"

#include "UUID.h"

namespace Vast {

	class Asset
	{
	public:
		Asset(AssetType assetType, const String& name, const Filepath& path, UUID uuid);

		UUID GetUUID() const { return m_UUID; }
		AssetType GetType() const { return m_AssetType; }
		const String& GetName() const { return m_Name; }
		const Filepath& GetPath() const { return m_Path; }
	protected:
		UUID m_UUID;
		AssetType m_AssetType;
		String m_Name;
		Filepath m_Path; // Relative to the Content folder
	};

}
