#pragma once

#include "AssetCore.h"

#include "UUID.h"

namespace Vast {

	class Asset
	{
		friend class AssetSerializer;
	public:
		Asset(AssetType assetType, const String& name, const Filepath& path, UUID uuid);

		virtual UUID GetUUID() const { return m_UUID; }
		virtual AssetType GetType() const { return m_AssetType; }
		virtual const char* GetTypeName() const { return "None"; }
		virtual const String& GetName() const { return m_Name; }
		virtual const Filepath& GetPath() const { return m_Path; }
		
		virtual void SetName(const String& name) { m_Name = name; }
		virtual void SetPath(const Filepath& path) { m_Path = path; }
	protected:
		UUID m_UUID;
		AssetType m_AssetType;
		String m_Name;
		Filepath m_Path; // Relative to the Content folder
	};

#define ASSET_TYPE_IMPL(type) static AssetType GetStaticType() { return type; }\
							  static const char* GetStaticTypeName() { return #type; }\
							  const char* GetTypeName() const override { return #type; }

}
