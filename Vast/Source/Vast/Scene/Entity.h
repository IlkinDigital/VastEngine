#pragma once

#include "Scene.h"

namespace Vast {
	
	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID handle, Scene* scene);
		
		template<typename Ty, typename... Args>
		Ty& AddComponent(Args&&... args)
		{
			VAST_CORE_ASSERT(!HasComponent<Ty>(), "Entity already has this component");
			Ty& component = m_Scene->m_Registry.emplace<Ty>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename Ty>
		Ty& GetComponent()
		{
			return m_Scene->m_Registry.get<Ty>(m_EntityHandle);
		}

		template<typename Ty>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<Ty>(m_EntityHandle);
		}

		EntityID GetID() const { return m_EntityHandle; }
	private:
		EntityID m_EntityHandle;
		Scene* m_Scene;
	};

}

