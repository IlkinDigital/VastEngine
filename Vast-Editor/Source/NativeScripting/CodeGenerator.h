#pragma once

#include "CoreMinimal.h"

#include "Project/Project.h"

namespace Vast {

	class CodeGenerator
	{
	public:
		CodeGenerator(const Ref<Project>& project)
			: m_Project(project) {}

		void GeneratePremakeFile();
		void GeneratePCH();
		void GenerateExportFiles();
	private:
		Ref<Project> m_Project;
	};

}
