#pragma once

#include "Clock/Timestep.h"
#include "SceneCore.h"
#include "Core/UUID.h"

#include "Camera/EditorCamera.h"

namespace Vast {
	
	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		static Ref<Scene> Clone(const Ref<Scene>& srcScene);

		Entity CreateEntity(const String& label);
		Entity CreateEntity(UUID uuid, const String& label);
		void DestroyEntity(const Entity& entity);
		void DuplicateEntity(Entity entity);

		Entity GetEntity(UUID id);

		void OnRuntimeUpdate(Timestep ts);
		void OnUpdate(Timestep ts, const EditorCamera& camera);
		void OnViewportResize(uint32 width, uint32 height);

		Entity GetPrimaryCamera();
		EntityRegistry& GetRegistry() { return m_Registry; }
	private:
		EntityRegistry m_Registry;
	};

}
