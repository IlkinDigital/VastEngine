#pragma once

#include "CoreMinimal.h"

#include "Project/Project.h"

namespace Vast {

	class ScriptCodeGenerator
	{
	public:
		ScriptCodeGenerator(const Ref<Project>& project)
			: m_Project(project) {}

		void GeneratePCH();
		void GenerateExportFiles();
	private:
		Ref<Project> m_Project;
	};

}
