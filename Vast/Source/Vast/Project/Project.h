#pragma once

#include "CoreMinimal.h"
#include "AssetManager/AssetManager.h"

namespace Vast {

	class Project
	{
	public:
		Project();
		Project(const String& name, const Filepath& projectPath);

		void Set(const String& name, const Filepath& projectPath);

		const String& GetName() const { return m_Name; }
		const Filepath& GetProjectPath() const { return m_ProjectPath; }
		const Filepath& GetScriptModulePath() const { return m_ScriptPath; }
		Filepath GetContentFolderPath() const { return m_ProjectPath / "Content"; }
		const Ref<AssetManager>& GetAssetManager() const { return m_AssetManager; }

		void SetProjectPath(const Filepath& path) { m_ProjectPath = path; InvalidateScriptPath(); }

		// Compiles project
		void Build();
	private:
		void InvalidateScriptPath();
	private:
		Ref<AssetManager> m_AssetManager = CreateRef<AssetManager>();

		String m_Name;
		Filepath m_ScriptPath;
		Filepath m_ProjectPath;
	};

}
