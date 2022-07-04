#include "vastpch.h"

#ifdef VAST_PLATFORM_WINDOWS

#include "Utils/System/System.h"

#include <shellapi.h>

namespace Vast {

	void System::RunCommand(const Filepath& directory, const String& command)
	{
		OPTICK_EVENT();

		WString fullCommand = L"/c ";
		fullCommand.append(command.begin(), command.end());
		ShellExecute(NULL, L"open", L"cmd", fullCommand.c_str(),
			directory.wstring().c_str(), SW_HIDE);
	}

}

#endif

