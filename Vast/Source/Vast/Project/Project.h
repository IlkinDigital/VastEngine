#pragma once

#include "CoreMinimal.h"
#include "AssetManager/AssetManager.h"

namespace Vast {

	class Project
	{
	public:
		static const Project& Get() { return *s_LatestProject; }
		static bool Exists() { return s_LatestProject != nullptr; }
		static Ref<AssetManager>& GetAssetManager() { return s_LatestProject->m_AssetManager; }
	public:
		Project();
		Project(const String& name, const Filepath& projectPath);

		void Set(const String& name, const Filepath& projectPath);

		const String& GetName() const { return m_Name; }
		const Filepath& GetProjectPath() const { return m_ProjectPath; }
		const Filepath& GetScriptModulePath() const { return m_ScriptPath; }
		Filepath GetContentFolderPath() const { return m_ProjectPath / "Content"; }
		const Ref<AssetManager>& GetCurrentAssetManager() const { return m_AssetManager; }

		void SetProjectPath(const Filepath& path) { m_ProjectPath = path; InvalidateScriptPath(); }

		// Compiles project
		void Build();
	private:
		void InvalidateScriptPath();
	private:
		Ref<AssetManager> m_AssetManager;

		String m_Name;
		Filepath m_ScriptPath;
		Filepath m_ProjectPath;

		inline static Project* s_LatestProject = nullptr;
	};

}
