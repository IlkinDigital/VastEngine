#pragma once

#include "Vast/Core/CoreMinimal.h"

namespace Vast {

	class Project
	{
	public:
		Project() = default;
		Project(const String& name, const Filepath& projectPath);

		void Set(const String& name, const Filepath& projectPath);

		const String& GetName() const { return m_Name; }
		const Filepath& GetProjectPath() const { return m_ProjectPath; }
		const Filepath& GetScriptModulePath() const { return m_ScriptPath; }
		Filepath GetContentFolderPath() const { return m_ProjectPath / "Content"; }

		void SetProjectPath(const Filepath& path) { m_ProjectPath = path; InvalidateScriptPath(); }

		// Compiles project
		void Build();
	private:
		void InvalidateScriptPath();
	private:
		String m_Name;
		Filepath m_ScriptPath;
		Filepath m_ProjectPath;
	};

}
