#pragma once

#include "AssetCore.h"

#include "UUID.h"

namespace Vast {

	class Asset
	{
	public:
		Asset(AssetType assetType, const String& name, const Filepath& path, UUID uuid);

		virtual UUID GetUUID() const { return m_UUID; }
		virtual AssetType GetType() const { return m_AssetType; }
		virtual const String& GetName() const { return m_Name; }
		virtual const Filepath& GetPath() const { return m_Path; }
	protected:
		UUID m_UUID;
		AssetType m_AssetType;
		String m_Name;
		Filepath m_Path; // Relative to the Content folder
	};

}
