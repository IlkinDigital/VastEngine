#pragma once

#include "CoreMinimal.h"

#include "Project/Project.h"

namespace Vast {

	class CodeGenerator
	{
	public:
		CodeGenerator(const Project& project)
			: m_Project(project) {}

		void GeneratePremakeFile();
		void GeneratePCHFiles();
		void GenerateExportFiles();
	private:
		const Project& m_Project;
	};

}
