#include "vastpch.h"
#include "ScriptEngine.h"

#include "Clock/Clock.h"
#include "ScriptCodeGenerator.h"
#include "Utils/System/System.h"

namespace Vast {

	ScriptEngine::ScriptEngine()
	{
		s_Instance = this;
	}

	void ScriptEngine::Init()
	{
	}

	void ScriptEngine::Shutdown()
	{
		m_ScriptBuffer.ClearBuffer();
		if (m_ScriptModule)
			m_ScriptModule->Clear();
	}

	void ScriptEngine::GenerateExportFiles()
	{
		OPTICK_EVENT();

		ScriptCodeGenerator gen(m_Project);
		gen.GenerateExportFiles();
	}

	void ScriptEngine::LoadModule()
	{
		OPTICK_EVENT();

		InvalidateScriptPath();
		m_ScriptModule = RuntimeModule::Create(m_ScriptPath);

		if (m_ScriptModule->IsLoaded())
		{
			InitModuleFunc = m_ScriptModule->LoadFunction<InitModuleFn>("InitModule");
			InitScriptsFunc = m_ScriptModule->LoadFunction<InitScriptsFn>("InitScripts");
			GetScriptsFunc = m_ScriptModule->LoadFunction<GetScriptsFn>("GetScripts");

			InitModuleFunc(Application::GetPointer());
			InitScriptsFunc();
			m_ScriptBuffer.SetBuffer(GetScriptsFunc());
		}
		else
			VAST_CORE_ERROR("Failed to load ScriptModule");
	}

	void ScriptEngine::BuildModule()
	{
		OPTICK_EVENT();

		GenerateExportFiles();

		m_ScriptModule->Clear();

		StringStream ss;
		ss << R"("C:/Program Files/Microsoft Visual Studio/2022/Community/Msbuild/Current/Bin/amd64/MSBuild.exe" )"
			<< (m_Project->GetProjectPath() / (m_Project->GetName() + ".vcxproj")).string()
			<< " -flp:logFile=Resources/BuildLogs/Build.log;verbosity=quiet"
			<< " -property:Configuration=Debug"
			<< " -property:Platform=x64 "
			<< " -property:TargetName=WackoDuel-" << Clock::DateTimeDesc();

		std::system(ss.str().c_str());
		//System::RunCommand(std::filesystem::current_path(), ss.str());
	}

	void ScriptEngine::InvalidateScriptPath()
	{
		OPTICK_EVENT();

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

		m_ScriptPath = m_Project->GetProjectPath();
		m_ScriptPath /= "Binaries";
		m_ScriptPath /= config + '-' + platform;
		m_ScriptPath /= m_Project->GetName();

		DArray<String> dllFiles;

		for (auto& path : std::filesystem::directory_iterator(m_ScriptPath))
		{
			if (path.path().extension() == ".dll")
			{
				dllFiles.emplace_back(path.path().filename().string());
			}
		}

		if (dllFiles.size())
			m_ScriptPath /= *std::max_element(dllFiles.begin(), dllFiles.end());
	}

}