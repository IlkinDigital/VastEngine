#pragma once

#include "Project.h"

namespace Vast {

	class ProjectGenerator
	{
	public:
		ProjectGenerator(const Ref<Project> project)
			: m_Project(project) {}
		
		void GenerateDirectories();
		void GeneratePremakeFile();
		void DownloadDependencies(const Filepath& dest);
	private:
		Ref<Project> m_Project;
	};

}
