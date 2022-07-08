#include "vastpch.h"
#include "AssetSerializer.h"

#include "SceneSerializer.h"
#include "SerializationCore.h"

#include "AssetManager/AssetTypes.h"

namespace Vast {

	AssetSerializer::AssetSerializer(const Ref<Project>& project, const Ref<Asset>& asset)
		: m_Project(project), m_Asset(asset)
	{
	}

	void AssetSerializer::Serialize()
	{
		OPTICK_EVENT();

		Package package;
		package.First = SerializeHead();

		switch (m_Asset->GetType())
		{
		case AssetType::Texture2D:
			package.Second = SerializeTexture();
			break;
		case AssetType::BoardFlipbook:
			package.Second = SerializeBoardFlipbook();
			break;
		case AssetType::Scene:
			package.Second = SerializeScene();
			break;
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			return;
		}

		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += m_Asset->GetPath().string() + ".asset";

		std::ofstream fs(fullPath, std::ios::binary);
		fs << FilePackager::Pack(package);
	}

	bool AssetSerializer::Deserialize(const Filepath& path)
	{
		OPTICK_EVENT();

		if (m_Package.First.empty())
		{	
			ReceivePackage(path);
			DeserializeHead(m_Package.First, path);
		}

		switch (m_Asset->GetType())
		{
		case AssetType::Texture2D:
			return DeserializeTexture(m_Package.Second);
		case AssetType::BoardFlipbook:
			return DeserializeBoardFlipbook(m_Package.Second);
		case AssetType::Scene:
			return DeserializeScene(m_Package.Second);
		default:
			VAST_CORE_ASSERT(false, "Vast doesn't support this AssetType");
			break;
		}

		return false;
	}

	AssetType AssetSerializer::SerializationType(const Filepath& path)
	{
		OPTICK_EVENT();
		
		if (m_Asset)
			return m_Asset->GetType();
		
		// Invalidated path
		Filepath invalPath = path;

		if (path.extension() == ".asset")
			invalPath.replace_extension("");

		ReceivePackage(invalPath);
		DeserializeHead(m_Package.First, invalPath);

		return m_Asset->GetType();
	}

	void AssetSerializer::ReceivePackage(const Filepath& path)
	{
		OPTICK_EVENT();

		Filepath fullPath = m_Project->GetContentFolderPath();
		fullPath += path.string() + ".asset";

		if (!std::filesystem::is_regular_file(fullPath))
		{
			VAST_CORE_ERROR("Can't deserialize asset, '{0}' path is invalid", fullPath.string());
			return;
		}

		String data(std::filesystem::file_size(fullPath), '\0');

		std::ifstream fs(fullPath, std::ios::binary);
		fs.read(data.data(), data.size());

		m_Package = FilePackager::Unpack(data);
	}

	String AssetSerializer::SerializeHead()
	{
		OPTICK_EVENT();

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
		OPTICK_EVENT();

		YAML::Node head = YAML::Load(source);

		UUID uuid(head["Asset"].as<uint64>());
		AssetType type = (AssetType)head["Type"].as<uint16>();
		String name = head["Name"].as<String>();

		switch (type)
		{
		case AssetType::Texture2D:
			m_Asset = CreateRef<Texture2DAsset>(name, assetPath, uuid);
			break;
		case AssetType::BoardFlipbook:
			m_Asset = CreateRef<BoardFlipbookAsset>(name, assetPath, uuid);
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
		OPTICK_EVENT();

		const auto& data = RefCast<Texture2DAsset>(m_Asset)->GetFileData();
		return String(data.begin(), data.end());
	}

	bool AssetSerializer::DeserializeTexture(const String& source)
	{
		OPTICK_EVENT();

		Ref<Texture2DAsset> ta = RefCast<Texture2DAsset>(m_Asset);
		ta->SetFileData(DArray<char>(source.begin(), source.end()));
		ta->SetTexture(Texture2D::Create(ta));

		return true;
	}

	String AssetSerializer::SerializeScene()
	{
		OPTICK_EVENT();

		Ref<SceneAsset> sa = RefCast<SceneAsset>(m_Asset);
		SceneSerializer ss(sa->GetScene(), m_Project);
		return ss.Serialize();
	}

	bool AssetSerializer::DeserializeScene(const String& source)
	{
		OPTICK_EVENT();

		Ref<SceneAsset> sa = RefCast<SceneAsset>(m_Asset);
		sa->SetScene(CreateRef<Scene>());

		SceneSerializer ss(sa->GetScene(), m_Project);
		return ss.Deserialize(source);
	}

	String AssetSerializer::SerializeBoardFlipbook()
	{
		OPTICK_EVENT();

		auto bfa = RefCast<BoardFlipbookAsset>(m_Asset);
		const auto& fb = bfa->GetFlipbook();

		YAML::Emitter out;
		
		out << YAML::BeginMap;

		out << YAML::Key << "FPS" << YAML::Value << fb->GetFPS();
		out << YAML::Key << "KeyFrames" << YAML::Value << YAML::BeginSeq;

		for (const auto& key : fb->GetKeyFrames())
		{
			out << YAML::BeginMap;

			Filepath path = key.Texture->GetFilepath();
			path.replace_extension("");
			out << YAML::Key << "FrameSource" << YAML::Value << path.string();

			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		return out.c_str();
	}

	bool AssetSerializer::DeserializeBoardFlipbook(const String& source)
	{
		OPTICK_EVENT();

		YAML::Node data = YAML::Load(source);
		
		auto fb = CreateRef<Board2D::Flipbook>();
		
		if (!data["FPS"])
			return false;
		fb->SetFPS(data["FPS"].as<float>());

		auto keyFrames = data["KeyFrames"];
		if (!keyFrames)
			return false;

		for (auto frame : keyFrames)
		{
			Ref<Asset> raw = AssetManager::Get()->GetAsset(frame["FrameSource"].as<String>());
			auto asset = RefCast<Texture2DAsset>(raw);
			fb->PushKeyFrame({ asset->GetTexture() });
		}

		RefCast<BoardFlipbookAsset>(m_Asset)->SetFlipbook(fb);

		return true;
	}

}
