#include "Project.h"

namespace Vast {

	Project::Project(const String& name, const Filepath& projectPath)
		: m_Name(name), m_ProjectPath(projectPath)
	{
		InvalidateScriptPath();
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