#include "vastpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Camera/Camera.h"

#include "Components.h"

#include "Renderer/Renderer2D.h"
#include "Scripting/ScriptBuffer.h"

namespace Vast {

	Scene::Scene()
	{
	}

	Entity Scene::CreateEntity(const String& label)
	{
		return CreateEntity(UUID(), label);
	}

	Entity Scene::CreateEntity(UUID uuid, const String& label)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = label.empty() ? "Nameless Entity" : label;
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity.GetHandle());
	}

	Entity Scene::GetEntity(UUID id)
	{
		auto view = m_Registry.view<IDComponent>();

		for (auto entityID : view)
		{
			if (id == view.get<IDComponent>(entityID).ID)
				return Entity(entityID, this);
		}

		VAST_CORE_ASSERT(false, "Entity with specified UUID doesn't exist");
		return Entity();
	}

	void Scene::OnRuntimeUpdate(Timestep ts)
	{
		/**
		* Call scripts
		*/

		m_Registry.view<NativeScriptComponent>().each([=](auto entityID, NativeScriptComponent& nsc)
			{
				if (nsc.IsBound)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity(entityID, this);
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				}
			});

		/**
		* Render RenderComponents
		*/

		Camera* mainCamera = nullptr;
		Mat4 cameraTransform;

		auto group = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.Transform();
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.view<TransformComponent, RenderComponent>();
			for (auto entity : group)
			{
				auto [transform, renderable] = group.get<TransformComponent, RenderComponent>(entity);

				if (renderable.Texture)
					Renderer2D::DrawQuad(transform.Transform(), renderable.Texture);
				else
					Renderer2D::DrawQuad(transform.Transform(), renderable.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdate(Timestep ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.view<TransformComponent, RenderComponent>();
		for (auto entity : group)
		{
			auto [transform, renderable] = group.get<TransformComponent, RenderComponent>(entity);

			if (renderable.Texture)
				Renderer2D::DrawQuad(transform.Transform(), renderable.Texture);
			else
				Renderer2D::DrawQuad(transform.Transform(), renderable.Color);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32 width, uint32 height)
	{
		auto group = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			auto& cc = group.get<CameraComponent>(entity);
			cc.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCamera()
	{
		auto group = m_Registry.view<CameraComponent>();
		for (auto entity : group)
		{
			const auto& camera = group.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity(entity, this);
		}
		
		return {};
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<uint64, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto entity : view)
		{
			UUID uuid = src.get<IDComponent>(entity).ID;
			VAST_CORE_ASSERT((enttMap.find(uuid) != enttMap.end()), "Entity doesn't exist in enttMap");
			entt::entity dstEntityID = enttMap.at(uuid);

			auto& component = src.get<Component>(entity);
			dst.emplace_or_replace<Component>(dstEntityID, component);
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>()); 
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());

		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<RenderComponent>(newEntity, entity);
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
	}

	Ref<Scene> Scene::Clone(const Ref<Scene>& srcScene)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		auto& srcRegistry = srcScene->m_Registry;
		auto& dstRegistry = newScene->m_Registry;

		std::unordered_map<uint64, entt::entity> enttMap;

		auto idView = srcRegistry.view<IDComponent>();
		for (auto entityID : idView)
		{
			UUID uuid = srcRegistry.get<IDComponent>(entityID).ID;
			const String& name = srcRegistry.get<TagComponent>(entityID).Tag;
			Entity newEntity = newScene->CreateEntity(uuid, name);
			enttMap[uuid] = newEntity.GetHandle();
		}

		CopyComponent<TransformComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<CameraComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<RenderComponent>(dstRegistry, srcRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstRegistry, srcRegistry, enttMap);

		return newScene;
	}
}