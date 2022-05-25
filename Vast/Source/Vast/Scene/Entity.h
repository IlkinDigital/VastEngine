#pragma once

#include "Scene.h"

#include "Components/TagComponent.h"

namespace Vast {
	
	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID handle, Scene* scene);
		
		template<typename Ty, typename... Args>
		Ty& AddComponent(Args&&... args);

		template<typename Ty, typename... Args>
		Ty& AddOrReplaceComponent(Args&&... args);

		template<typename Ty>
		void RemoveComponent();

		template<typename Ty>
		Ty& GetComponent();

		template<typename Ty>
		bool HasComponent();

		bool IsValid() const { return m_EntityHandle != entt::null; }
		EntityID GetHandle() const { return m_EntityHandle; }

		const String& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator == (const Entity& other) { return m_EntityHandle == other.m_EntityHandle; }
	private:
		EntityID m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

	template<typename Ty, typename ...Args>
	inline Ty& Entity::AddComponent(Args && ...args)
	{
		VAST_CORE_ASSERT(!HasComponent<Ty>(), "Entity already has this component");
		Ty& component = m_Scene->GetRegistry().emplace<Ty>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename Ty, typename ...Args>
	inline Ty& Entity::AddOrReplaceComponent(Args && ...args)
	{
		Ty& component = m_Scene->GetRegistry().emplace_or_replace<Ty>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename Ty>
	inline void Entity::RemoveComponent()
	{
		m_Scene->GetRegistry().remove<Ty>(m_EntityHandle);
	}

	template<typename Ty>
	inline Ty& Entity::GetComponent()
	{
		return m_Scene->GetRegistry().get<Ty>(m_EntityHandle);
	}

	template<typename Ty>
	inline bool Entity::HasComponent()
	{
		return m_Scene->GetRegistry().all_of<Ty>(m_EntityHandle);
	}
}

