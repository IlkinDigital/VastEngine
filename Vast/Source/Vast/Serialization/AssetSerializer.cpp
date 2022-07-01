#include "vastpch.h"
#include "AssetSerializer.h"

#include "AssetManager/TextureAsset.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Vast {

	AssetSerializer::AssetSerializer(const Ref<Project>& project, const Ref<Asset>& asset)
		: m_Project(project), m_Asset(asset)
	{
	}

	void AssetSerializer::Serialize()
	{
		switch (m_Asset->GetType())
		{
		case AssetType::Texture:
			SerializeTexture();
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}
	}

	void AssetSerializer::Deserialize(const Filepath& path)
	{
		DeserializeHead(path);

		switch (m_Asset->GetType())
		{
		case AssetType::Texture:
			DeserializeTexture(path);
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}
	}

	void AssetSerializer::SerializeHead(YAML::Emitter& out)
	{
		out << YAML::BeginMap;

		/*
		 Asset: - UUID
			- Type
			- Name
		*/

		out << YAML::Key << "Asset" << YAML::Value << m_Asset->GetUUID();
		out << YAML::Key << "Type" << YAML::Value << (uint16)m_Asset->GetType();
		out << YAML::Key << "Name" << YAML::Value << m_Asset->GetName();

		out << YAML::EndMap;
	}

	void AssetSerializer::DeserializeHead(const Filepath& path)
	{
		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += path;

		std::ifstream fs(fullPath);
		StringStream ss;
		ss << fs.rdbuf();

		String str = ss.str();
		for (auto it = str.begin(); it != str.end(); it++)
		{
			if (*it == '\0')
			{
				it = str.erase(it, str.end());
				break;
			}
		}

		YAML::Node head = YAML::Load(str);

		UUID uuid(head["Asset"].as<uint64>());
		AssetType type = (AssetType)head["Type"].as<uint16>();
		String name = head["Name"].as<String>();

		switch (type)
		{
		case AssetType::Texture:
			m_Asset = CreateRef<TextureAsset>(name, path, uuid);
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}
			
	}

	void AssetSerializer::SerializeTexture()
	{
		YAML::Emitter out;
		SerializeHead(out);

		Filepath path = m_Project->GetContentFolderPath();
		path += m_Asset->GetPath();
		std::ofstream fs(path, std::ios::binary);
		const char* head = out.c_str();

		fs.write(head, std::strlen(head));
		fs.write("\0", 1);
	
		const auto& data = RefCast<TextureAsset>(m_Asset)->GetFileData();
		fs.write(data.data(), data.size());

		fs.close();
	}

	bool AssetSerializer::DeserializeTexture(const Filepath& path)
	{
		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += path;

		if (!std::filesystem::is_regular_file(fullPath))
		{
			VAST_CORE_ERROR("Can't deserialize texture, '{0}' path is invalid", path.string());
			return false;
		}

		std::ifstream fs(fullPath, std::ios::binary);
		DArray<char> data(std::filesystem::file_size(fullPath), '\0');
		fs.read(data.data(), data.size());

		int i = 0;
		for (; data[i] != '\0' && i < data.size(); i++) {}
		i++;
	
		DArray<char> fileData(data.begin() + i, data.end());
		
		Ref<TextureAsset> ta = RefCast<TextureAsset>(m_Asset);
		ta->SetFileData(fileData);

		return true;
	}

}
