#pragma once

#include "Scene/Entity.h"
#include "AssetManager/Asset.h"
#include "AssetManager/AssetTypes.h"
#include "Project/Project.h"

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

		template<typename Ty>
		Ref<Ty> LoadAsset(const Filepath& path)
		{
			VAST_CORE_ASSERT(m_Project->GetAssetManager(), "AssetManager is null");
			return RefCast<Ty>(RefCast<Ty>(m_Project->GetAssetManager()->GetAsset(path))->Clone());
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	private:
		void SetProject(const Ref<Project>& project) { m_Project = project; }
	private:
		Ref<Project> m_Project;
		bool m_Destroyed = false;
		Entity m_Entity;
	};

}