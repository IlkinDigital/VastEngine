#include "vastpch.h"
#include "SceneSerializer.h"

#include "Scene/Entity.h"
#include "Scene/Components.h"

#include "SerializationCore.h"

namespace Vast {

	void SceneSerializer::Serialize(const String& filepath)
	{
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

		std::ofstream fs(filepath);
		fs << out.c_str();
	}

	bool SceneSerializer::Deserialize(const String& filepath)
	{
		std::ifstream fs(filepath);
		StringStream strStream;
		strStream << fs.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

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
					
					auto texturePath = renderComponent["Texture"].as<String>();
					if (texturePath != "")
						rc.Texture = Texture2D::Create(texturePath);
				}

				/**
				* Native Script Component
				*/
				auto nscriptComponent = entity["NativeScriptComponent"];
				if (nscriptComponent)
				{
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();

					nsc.Name = nscriptComponent["Name"].as<String>();

					// Instantiation from ScriptBuffer does the client application
				}
			}
		}

		return true;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
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
				out << YAML::Key << "Texture" << YAML::Value << ((rc.Texture) ? rc.Texture->GetFilepath() : "");
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