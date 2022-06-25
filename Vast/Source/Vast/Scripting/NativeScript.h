#pragma once

#include "Scene/Entity.h"

#define REG_CLASS(uclass)

namespace Vast {

	class NativeScript
	{
		friend class Scene;
	public:
		virtual ~NativeScript() = default;

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

		void DestroySelf();

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	private:
		bool m_Destroyed = false;
		Entity m_Entity;
	};

}