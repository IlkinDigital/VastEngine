#pragma once

#include "Entity.h"

namespace Vast {

	class ScriptableEntity
	{
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;

		template<typename Ty>
		Ty& GetComponent()
		{
			return m_Entity.GetComponent<Ty>();
		}

		Entity GetEntity(UUID id)
		{
			return m_Entity.m_Scene->GetEntity(id);
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
	};

}