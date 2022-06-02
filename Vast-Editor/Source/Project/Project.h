#pragma once

#include "Vast/Core/CoreMinimal.h"

namespace Vast {

	class Project
	{
	public:
		Project(const Filepath& projectPath)
			: m_ProjectPath(projectPath) {}
	private:
		Filepath m_ProjectPath;
	};

}
