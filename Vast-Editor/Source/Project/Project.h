#pragma once

#include "Vast/Core/CoreMinimal.h"

namespace Vast {

	class Project
	{
	public:
		Project(const String& name, const Filepath& projectPath);

		const Filepath& GetPath() const { return m_ProjectPath; }
		const Filepath& GetScriptModulePath();
	private:
		String m_Name;
		Filepath m_ScriptPath;
		Filepath m_ProjectPath;
	};

}
