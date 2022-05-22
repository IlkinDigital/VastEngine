#include "vastpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Camera/Camera.h"

#include "Components.h"

#include "Renderer/Renderer2D.h"

namespace Vast {

	Entity Scene::CreateEntity(const String& label)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = label.empty() ? "Nameless Entity" : label;
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity.GetID());
	}

	void Scene::OnScenePlay(Timestep ts)
	{
		/**
		* Call scripts
		*/

		m_Registry.view<NativeScriptComponent>().each([=](auto entityID, NativeScriptComponent& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(entityID, this);
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
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
				break;
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

}