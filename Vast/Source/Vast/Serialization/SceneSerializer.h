#pragma once

#include "Scene/Scene.h"
#include "Project/Project.h"

namespace YAML {
	class Emitter;
}

namespace Vast {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene, const Ref<Project>& project)
			: m_Scene(scene), m_Project(project) {}

		void Serialize(const Filepath& filepath);
		bool Deserialize(const Filepath& filepath);
	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);
		
		template<typename Ty, typename Fn>
		static void SerializeComponent(YAML::Emitter& out, const String& name, Entity entity, Fn function);
	private:
		Ref<Scene> m_Scene;
		Ref<Project> m_Project;
	};

}
