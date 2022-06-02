#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class CodeGenerator
	{
	public:
		CodeGenerator(const Filepath& directory)
			: m_Directory(directory) {}

		void GenerateProjectFile();
		void GenerateExportFile();
	private:
		Filepath m_Directory;
	};

}
