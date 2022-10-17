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
		SceneSerializer(const Ref<Scene>& scene)
			: m_Scene(scene) {}

		void Serialize(const Filepath& filepath); // DEPRECATED: Use asset serializer
		bool Deserialize(const Filepath& filepath); // DEPRECATED: Use asset serializer

		String Serialize();
		bool Deserialize(const String& source);
	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);
		
		template<typename Ty, typename Fn>
		static void SerializeComponent(YAML::Emitter& out, const String& name, Entity entity, Fn function);
	private:
		Ref<Scene> m_Scene;
	};

}
