#include "vastpch.h"
#include "WindowsModule.h"

namespace Vast {

#ifdef VAST_PLATFORM_WINDOWS
	Ref<RuntimeModule> RuntimeModule::Create(const Filepath& filepath)
	{
		return CreateRef<WindowsModule>(filepath);
	}
#endif

	WindowsModule::WindowsModule(const Filepath& filepath)
	{
		m_ModuleHandle = LoadLibraryA(filepath.string().c_str());
		VAST_CORE_ASSERT(m_ModuleHandle, "Couldn't load module");
	}

	WindowsModule::~WindowsModule()
	{
		//Clean();
	}

	void WindowsModule::Clean()
	{
		if (m_ModuleHandle)
			FreeLibrary(m_ModuleHandle);
	}

	RuntimeModule::FnPtr WindowsModule::OpenFunction(const String& name)
	{
		auto impFunc = GetProcAddress(m_ModuleHandle, name.c_str());
		VAST_CORE_ASSERT(impFunc, "Couldn't locate function");
		return impFunc;
	}
}
