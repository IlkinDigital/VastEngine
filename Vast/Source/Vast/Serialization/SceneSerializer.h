#pragma once

#include "Scene/Scene.h"

namespace YAML {
	class Emitter;
}

namespace Vast {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene)
			: m_Scene(scene) {}

		void Serialize(const String& filepath);
		bool Deserialize(const String& filepath);
	private:
		static void SerializeEntity(YAML::Emitter& out, Entity entity);
		
		template<typename Ty, typename Fn>
		static void SerializeComponent(YAML::Emitter& out, const String& name, Entity entity, Fn function);
	private:
		Ref<Scene> m_Scene;
	};

}
