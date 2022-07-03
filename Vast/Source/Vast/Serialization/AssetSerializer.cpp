#include "vastpch.h"
#include "AssetSerializer.h"

#include "SceneSerializer.h"
#include "SerializationCore.h"

#include "AssetManager/Texture2DAsset.h"
#include "AssetManager/SceneAsset.h"

#include "Utils/FileIO/Packager/FilePackager.h"

namespace Vast {

	AssetSerializer::AssetSerializer(const Ref<Project>& project, const Ref<Asset>& asset)
		: m_Project(project), m_Asset(asset)
	{
	}

	void AssetSerializer::Serialize()
	{
		Package package;
		package.First = SerializeHead();

		switch (m_Asset->GetType())
		{
		case AssetType::Texture2D:
			package.Second = SerializeTexture();
			break;
		case AssetType::Scene:
			package.Second = SerializeScene();
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			return;
		}

		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += m_Asset->GetPath();

		std::ofstream fs(fullPath, std::ios::binary);
		fs << FilePackager::Pack(package);
	}

	bool AssetSerializer::Deserialize(const Filepath& path)
	{
		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += path;

		if (!std::filesystem::is_regular_file(fullPath))
		{
			VAST_CORE_ERROR("Can't deserialize texture, '{0}' path is invalid", path.string());
			return false;
		}

		String data(std::filesystem::file_size(fullPath), '\0');

		std::ifstream fs(fullPath, std::ios::binary);
		fs.read(data.data(), data.size());

		Package package = FilePackager::Unpack(data);

		DeserializeHead(package.First, path);

		switch (m_Asset->GetType())
		{
		case AssetType::Texture2D:
			return DeserializeTexture(package.Second);
			break;
		case AssetType::Scene:
			return DeserializeScene(package.Second);
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}

		return false;
	}

	String AssetSerializer::SerializeHead()
	{
		YAML::Emitter out;

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
		return out.c_str();
	}

	void AssetSerializer::DeserializeHead(const String& source, const Filepath& assetPath)
	{
		YAML::Node head = YAML::Load(source);

		UUID uuid(head["Asset"].as<uint64>());
		AssetType type = (AssetType)head["Type"].as<uint16>();
		String name = head["Name"].as<String>();

		switch (type)
		{
		case AssetType::Texture2D:
			m_Asset = CreateRef<Texture2DAsset>(name, assetPath, uuid);
			break;
		case AssetType::Scene:
			m_Asset = CreateRef<SceneAsset>(name, assetPath, uuid);
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}
			
	}

	String AssetSerializer::SerializeTexture()
	{
		const auto& data = RefCast<Texture2DAsset>(m_Asset)->GetFileData();
		return String(data.begin(), data.end());
	}

	bool AssetSerializer::DeserializeTexture(const String& source)
	{
		Ref<Texture2DAsset> ta = RefCast<Texture2DAsset>(m_Asset);
		ta->SetFileData(DArray<char>(source.begin(), source.end()));
		ta->SetTexture(Texture2D::Create(ta));

		return true;
	}

	String AssetSerializer::SerializeScene()
	{
		Ref<SceneAsset> sa = RefCast<SceneAsset>(m_Asset);
		SceneSerializer ss(sa->GetScene(), m_Project);
		return ss.Serialize();
	}

	bool AssetSerializer::DeserializeScene(const String& source)
	{
		Ref<SceneAsset> sa = RefCast<SceneAsset>(m_Asset);
		sa->SetScene(CreateRef<Scene>());

		SceneSerializer ss(sa->GetScene(), m_Project);
		return ss.Deserialize(source);
	}

	String AssetSerializer::SerializeBoardFlipbook()
	{
		return "";
	}

	bool AssetSerializer::DeserializeBoardFlipbook(const String& source)
	{
		return false;
	}

}
