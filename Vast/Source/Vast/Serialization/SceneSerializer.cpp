#include "vastpch.h"
#include "SceneSerializer.h"

#include "SerializationCore.h"

#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scripting/ScriptEngine.h"

#include "Utils/FileIO/FileIO.h"

#include "AssetManager/AssetTypes.h"

namespace Vast {

	void SceneSerializer::Serialize(const Filepath& filepath)
	{
		std::ofstream fs(filepath);
		fs << Serialize();
	}

	bool SceneSerializer::Deserialize(const Filepath& filepath)
	{
		std::ifstream fs(filepath);
		fs.open(filepath);
		StringStream ss;
		ss << fs.rdbuf();

		return Deserialize(ss.str());
	}

	String SceneSerializer::Serialize()
	{
		OPTICK_EVENT();

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->GetRegistry().each([&](auto entityID)
			{
				Entity entity(entityID, m_Scene.get());
				if (!entity.IsValid())
				{
					VAST_CORE_ASSERT(false, "Failed to serialize scene: Invalid entity");
					return;
				}

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		return String(out.c_str());
	}

	bool SceneSerializer::Deserialize(const String& source)
	{
		OPTICK_EVENT();

		YAML::Node data = YAML::Load(source);
		if (!data["Scene"])
			return false;

		String sceneName = data["Scene"].as<String>();
		VAST_CORE_TRACE("Deserializing scene: {0}", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				/**
				* UUID Component
				*/

				UUID uuid(entity["Entity"].as<uint64>());

				/**
				* Tag Component
				*/

				String tag;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					tag = tagComponent["Tag"].as<String>();

				VAST_CORE_TRACE("Deserializing entity with UUID = {0}, Tag = {1}", uuid, tag);

				Entity deserializedEntity = m_Scene->CreateEntity(uuid, tag);

				/**
				* Transform Component
				*/
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<Vector3>();
					tc.Rotation = transformComponent["Rotation"].as<Vector3>();
					tc.Scale = transformComponent["Scale"].as<Vector3>();
				}

				/**
				* Camera Component
				*/
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
				}

				/**
				* Render Component
				*/
				auto renderComponent = entity["RenderComponent"];
				if (renderComponent)
				{
					auto& rc = deserializedEntity.AddComponent<RenderComponent>();

					rc.Color = renderComponent["Color"].as<Vector4>();

					Filepath path = renderComponent["Texture"].as<String>();
					if (path != "")
					{
						if (path.is_absolute())
						{
							rc.Texture = Texture2D::Create(path);
						}
						else
						{
							const auto& asset = Project::GetAssetManager()->GetAsset(path);
							if (asset->GetType() == AssetType::Texture2D)
								rc.Texture = RefCast<Texture2DAsset>(asset)->GetTexture();
						}
					}
				}

				/**
				* Board Render Component
				*/
				auto boardRenderComponent = entity["BoardRenderComponent"];
				if (boardRenderComponent)
				{
					auto& brc = deserializedEntity.AddComponent<BoardRenderComponent>();

					brc.Color = boardRenderComponent["Color"].as<Vector4>();

					Filepath path = boardRenderComponent["Sprite"].as<String>();
					if (path != "")
					{
						const auto& asset = Project::GetAssetManager()->GetAsset(path);
						if (asset->GetType() == AssetType::BoardSprite)
							brc.Sprite = RefCast<BoardSpriteAsset>(asset)->GetSprite();
					}
				}

				/**
				* Sprite Component
				*/
				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& sc = deserializedEntity.AddComponent<SpriteComponent>();

					sc.Color = spriteComponent["Color"].as<Vector4>();

					Filepath path = spriteComponent["Flipbook"].as<String>();
					if (path != "")
					{
						sc.Flipbook = RefCast<BoardFlipbookAsset>(Project::GetAssetManager()->GetAsset(path)->Clone());
					}
				}

				/**
				* Native Script Component
				*/
				auto nscriptComponent = entity["NativeScriptComponent"];
				if (nscriptComponent)
				{
					auto nsc = ScriptEngine::Get()->GetScriptBuffer().FindByName(nscriptComponent["Name"].as<String>());
					deserializedEntity.AddComponent<NativeScriptComponent>(nsc);
				}
			}
		}

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		OPTICK_EVENT();

		out << YAML::BeginMap;
		out << YAML::Key << "Entity";
		out << YAML::Value << entity.GetComponent<IDComponent>().ID; // UUID goes here

		SerializeComponent<TagComponent>(out, "TagComponent", entity, [&](TagComponent& tc)
			{
				out << YAML::Key << "Tag" << YAML::Value << tc.Tag;
			});

		SerializeComponent<TransformComponent>(out, "TransformComponent", entity, [&](TransformComponent& tc)
			{
				out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
				out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			});

		SerializeComponent<CameraComponent>(out, "CameraComponent", entity, [&](CameraComponent& cc)
			{
				auto& camera = cc.Camera;

				out << YAML::Key << "Camera";
				out << YAML::Value;
				out << YAML::BeginMap; // Camera
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
				out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
				out << YAML::EndMap; // Camera
				
				out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			});

		SerializeComponent<RenderComponent>(out, "RenderComponent", entity, [&](RenderComponent& rc)
			{
				out << YAML::Key << "Color" << YAML::Value << rc.Color;
				if (rc.Texture)
				{
					Filepath path = rc.Texture->GetFilepath();
					path.replace_extension("");
					out << YAML::Key << "Texture" << YAML::Value << path.string();
				}
				else
					out << YAML::Key << "Texture" << YAML::Value << "";
			});

		SerializeComponent<BoardRenderComponent>(out, "BoardRenderComponent", entity, [&](BoardRenderComponent& brc)
			{
				out << YAML::Key << "Color" << YAML::Value << brc.Color;
				if (brc.Sprite)
				{
					out << YAML::Key << "Sprite" << YAML::Value << Project::GetAssetManager()->FindAsset(brc.Sprite)->GetPath().string();
				}
				else
					out << YAML::Key << "Sprite" << YAML::Value << "";
 			});

		SerializeComponent<SpriteComponent>(out, "SpriteComponent", entity, [&](SpriteComponent& sc)
			{
				out << YAML::Key << "Color" << YAML::Value << sc.Color;
				if (sc.Flipbook)
				{
					out << YAML::Key << "Flipbook" << YAML::Value << sc.Flipbook->GetPath().string();
				}
				else
					out << YAML::Key << "Flipbook" << YAML::Value << "";
			});

		SerializeComponent<NativeScriptComponent>(out, "NativeScriptComponent", entity, [&](NativeScriptComponent& nsc)
			{
				out << YAML::Key << "Name" << YAML::Value << nsc.Name;
			});

		out << YAML::EndMap;
	}

	template<typename Ty, typename Fn>
	inline void Vast::SceneSerializer::SerializeComponent(YAML::Emitter& out, const String& name, Entity entity, Fn function)
	{

		if (entity.HasComponent<Ty>()) // TODO: Consider removing
		{
			out << YAML::Key << name;
			out << YAML::BeginMap;

			function(entity.GetComponent<Ty>());

			out << YAML::EndMap;
		}
	}

}