#pragma once

#include "Scene.h"

namespace Vast {
	
	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID handle, Scene* scene);
		
		template<typename Ty, typename... Args>
		Ty& AddComponent(Args&&... args);

		template<typename Ty>
		Ty& GetComponent();

		template<typename Ty>
		bool HasComponent();

		bool IsValid() const { return m_EntityHandle != entt::null; }
		EntityID GetID() const { return m_EntityHandle; }
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

