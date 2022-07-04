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
		OPTICK_EVENT();

		Clear(); // TODO: Maybe remove
		String path = filepath.string();
		m_ModuleHandle = LoadLibraryA(path.c_str());

		if (!m_ModuleHandle)
		{
			VAST_CORE_ERROR("Couldn't load module from '{0}'", path);
			m_IsLoaded = false;
		}
		else
			m_IsLoaded = true;
	}

	WindowsModule::~WindowsModule()
	{
		//Clean();
	}

	void WindowsModule::Clear()
	{
		OPTICK_EVENT();

		LPWSTR filename = new WCHAR[128]{};
		GetModuleFileName(m_ModuleHandle, filename, 127);
		if (filename[0])
		{
			FreeLibrary(m_ModuleHandle);
			char buffer[128]{};
			wcstombs(buffer, filename, 128);
			VAST_CORE_WARN("Script Module - {0} - has been cleared", buffer);
		}
		delete[] filename;
	}

	RuntimeModule::FnPtr WindowsModule::OpenFunction(const String& name)
	{
		OPTICK_EVENT();

		auto impFunc = GetProcAddress(m_ModuleHandle, name.c_str());
		VAST_CORE_ASSERT(impFunc, "Couldn't locate function");
		return impFunc;
	}
}
