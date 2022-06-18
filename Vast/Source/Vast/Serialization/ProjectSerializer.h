#pragma once

#include "Project/Project.h"

namespace YAML {
	class Emitter;
}

namespace Vast {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(const Ref<Project>& project)
			: m_Project(project) {}

		void Serialize(const Filepath& filepath);
		bool Deserialize(const Filepath& filepath);
	private:
		Ref<Project> m_Project;

		static String s_ProjectFile;
	};

}
