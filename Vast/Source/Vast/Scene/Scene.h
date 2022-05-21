#pragma once

#include "Clock/Timestep.h"
#include "SceneCore.h"


namespace Vast {
	
	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		Entity CreateEntity(const String& label);
		void DestroyEntity(const Entity& entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);

		Entity GetPrimaryCamera();

		EntityRegistry& GetRegistry() { return m_Registry; }
	private:
		EntityRegistry m_Registry;
	};

}
