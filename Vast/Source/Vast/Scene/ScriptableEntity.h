#pragma once

#include "Entity.h"

#define REG_CLASS(uclass)

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

		DArray<Entity> GetEntityByName(const String& name);
		Entity CreateEntity(const String& name);

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	private:
		Entity m_Entity;
	};

}