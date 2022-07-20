#pragma once

#include "ApplicationCore/Application.h"
#include "Project/Project.h"
#include "Module/RuntimeModule.h"
#include "ScriptBuffer.h"

namespace Vast {

	class ScriptEngine
	{
	public:
		ScriptEngine();
		ScriptEngine(const Ref<Project>& project)
			: m_Project(project) { ScriptEngine(); }

		static ScriptEngine* Get() { return s_Instance; }

		void SetProject(const Ref<Project>& project) { m_Project = project; }

		ScriptBuffer& GetScriptBuffer() { return m_ScriptBuffer; }

		void Init();
		void Shutdown();

		void GenerateExportFiles();
		
		void LoadModule();
		void BuildModule();
	private:
		void InvalidateScriptPath();
	private:
		Ref<Project> m_Project;
		ScriptBuffer m_ScriptBuffer;
		Ref<RuntimeModule> m_ScriptModule;
		Filepath m_ScriptPath;

		/**
		* Imported functions from ScriptModule
		*/

		typedef void(*InitScriptsFn)();
		typedef void(*InitModuleFn)(Application*);
		typedef const DArray<NativeScriptComponent>& (*GetScriptsFn)();

		InitModuleFn InitModuleFunc;
		InitScriptsFn InitScriptsFunc;
		GetScriptsFn GetScriptsFunc;
	private:
		inline static ScriptEngine* s_Instance = nullptr;
	};

}
