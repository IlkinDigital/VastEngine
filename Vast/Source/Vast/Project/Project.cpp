#include "vastpch.h"
#include "Project.h"

namespace Vast {

	Project::Project()
	{
		m_AssetManager = CreateRef<AssetManager>();
		AssetManager::Set(m_AssetManager);
	}

	Project::Project(const String& name, const Filepath& projectPath)
	{
		m_AssetManager = CreateRef<AssetManager>();
		AssetManager::Set(m_AssetManager);
		Set(name, projectPath);
	}

	void Project::Set(const String& name, const Filepath& projectPath)
	{
		m_Name = name;
		m_ProjectPath = projectPath;
		InvalidateScriptPath();
	}

	// TODO: Multi compiler support (only supports MSVC compiler)
	void Project::Build()
	{
		StringStream ss;
		ss << R"("C:/Program Files/Microsoft Visual Studio/2022/Community/Msbuild/Current/Bin/amd64/MSBuild.exe" )"
			<< (m_ProjectPath / (m_Name + ".vcxproj")).string()
			<< " -property:Configuration=Debug -property:Platform=x64";

		std::system(ss.str().c_str());
	}

	void Project::InvalidateScriptPath()
	{
#ifdef VAST_PLATFORM_WINDOWS
		const char* platform = "windows-x86_64";
#else
		const char* platform = "";
#endif
#ifdef VAST_CONFIG_DEBUG
		String config = "Debug";
#elif defined VAST_CONFIG_RELEASE
		String config = "Release";
#elif defined VAST_CONFIG_DISTRIBUTION
		String config = "Distribution";
#endif

		m_ScriptPath = m_ProjectPath;
		m_ScriptPath /= "Binaries";
		m_ScriptPath /= config + '-' + platform;
		m_ScriptPath /= m_Name;
		m_ScriptPath /= m_Name + ".dll";
	}

}