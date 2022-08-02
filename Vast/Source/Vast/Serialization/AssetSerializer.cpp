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
		case AssetType::BoardSpriteSheet:
			package.Second = SerializeBoardSpriteSheet();
			break;
		case AssetType::BoardSprite:
			package.Second = SerializeBoardSprite();
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
		case AssetType::BoardSpriteSheet:
			return DeserializeBoardSpriteSheet(m_Package.Second);
		case AssetType::BoardSprite:
			return DeserializeBoardSprite(m_Package.Second);
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
		case AssetType::BoardSpriteSheet:
			m_Asset = CreateRef<BoardSpriteSheetAsset>(name, assetPath, uuid);
			break;
		case AssetType::BoardSprite:
			m_Asset = CreateRef<BoardSpriteAsset>(name, assetPath, uuid);
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

	String AssetSerializer::SerializeBoardSpriteSheet()
	{
		auto bssa = RefCast<BoardSpriteSheetAsset>(m_Asset);

		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "Stride" << YAML::Value << bssa->GetSpriteSheet()->GetStride();

		out << YAML::EndMap;

		AssetSerializer as(m_Project, bssa->GetTextureAsset());
		String textureSource = as.SerializeTexture();

		return FilePackager::Pack({ out.c_str(), textureSource });
	}

	bool AssetSerializer::DeserializeBoardSpriteSheet(const String& source)
	{
		Package pack = FilePackager::Unpack(source);

		YAML::Node data = YAML::Load(pack.First);

		auto stride = data["Stride"];
		if (!stride)
			return false;
		Vector2 ssStride = stride.as<Vector2>();

		Ref<Texture2DAsset> texAsset = CreateRef<Texture2DAsset>(m_Asset->GetName(), m_Asset->GetPath(), m_Asset->GetUUID());
		AssetSerializer as(m_Project, texAsset);
		as.DeserializeTexture(pack.Second);

		auto asset = RefCast<BoardSpriteSheetAsset>(m_Asset);
		auto ss = CreateRef<Board2D::SpriteSheet>(texAsset->GetTexture(), ssStride);
		asset->SetSpriteSheet(ss);
		asset->SetTextureAsset(RefCast<Texture2DAsset>(as.GetAsset()));
			
		return true;
	}

	String AssetSerializer::SerializeBoardSprite()
	{
		OPTICK_EVENT();

		Ref<BoardSpriteAsset> bsa = RefCast<BoardSpriteAsset>(m_Asset);
		const auto& sprite = bsa->GetSprite();

		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "SpriteSheet" << YAML::Value << bsa->GetPath().string();

		out << YAML::Key << "UVCoordinates" << YAML::Value << YAML::BeginSeq;

		for (const auto& coord : sprite->GetUVCoords())
		{
			out << coord;
		}

		out << YAML::EndSeq;

		out << YAML::EndMap;

		return out.c_str();
	}

	bool AssetSerializer::DeserializeBoardSprite(const String& source)
	{
		Package pack = FilePackager::Unpack(source);

		YAML::Node data = YAML::Load(pack.First);

		auto spriteSheet = data["SpriteSheet"];
		auto uvs = data["UVCoordinates"];
		if (!spriteSheet || !uvs)
			return false;

		SArray<Vector2, 2> coords;
		int i = 0;
		for (const auto& uv : uvs)
		{
			coords[i] = uv.as<Vector2>();
			i++;
		}

		auto asset = RefCast<BoardSpriteAsset>(m_Asset);
		auto sprite = Board2D::Sprite::Create(
			RefCast<BoardSpriteSheetAsset>(AssetManager::Get()->GetAsset(spriteSheet.as<String>()))->GetSpriteSheet(),
			coords);
		asset->SetSprite(sprite);

		return true;
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

			Filepath path = key.Sprite->GetTexture()->GetFilepath();
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
			auto asset = RefCast<BoardSpriteAsset>(raw);
			fb->PushKeyFrame({ asset->GetSprite() });
		}

		RefCast<BoardFlipbookAsset>(m_Asset)->SetFlipbook(fb);

		return true;
	}

}
