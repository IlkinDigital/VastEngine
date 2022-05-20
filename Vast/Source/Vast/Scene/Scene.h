#pragma once

#include "Clock/Timestep.h"
#include "SceneCore.h"

namespace Vast {

	class Scene
	{
		friend class Entity;
	public:
		Scene() = default;

		Entity CreateEntity(const String& label);
		void DestroyEntity(const Entity& entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32 width, uint32 height);
	private:
		EntityRegistry m_Registry;
	};

}
