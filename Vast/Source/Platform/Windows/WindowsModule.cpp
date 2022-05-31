#include "vastpch.h"
#include "WindowsModule.h"

namespace Vast {

#ifdef VAST_PLATFORM_WINDOWS
	Ref<RuntimeModule> Vast::RuntimeModule::Create(const String& filepath)
	{
		return CreateRef<WindowsModule>(filepath);
	}
#endif

	WindowsModule::WindowsModule(const String& filepath)
	{
		m_ModuleHandle = LoadLibraryA(filepath.c_str());
		VAST_CORE_ASSERT(m_ModuleHandle, "Couldn't load module");
	}

	WindowsModule::~WindowsModule()
	{
		//if (m_ModuleHandle)
			//FreeLibrary(m_ModuleHandle);
	}

	RuntimeModule::FnPtr WindowsModule::OpenFunction(const String& name)
	{
		auto impFunc = GetProcAddress(m_ModuleHandle, name.c_str());
		VAST_CORE_ASSERT(impFunc, "Couldn't locate function");
		return impFunc;
	}
}
