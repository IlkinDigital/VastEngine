#pragma once

#include "CoreMinimal.h"

#include "Project/Project.h"

namespace Vast {

	class CodeGenerator
	{
	public:
		CodeGenerator(const Project& project)
			: m_Project(project) {}

		void GenerateProjectFile();
		void GenerateExportFile();
	private:
		const Project& m_Project;
	};

}
