#pragma once

#include "Module/RuntimeModule.h"

namespace Vast {

	class WindowsModule : public RuntimeModule
	{
	public:
		WindowsModule(const String& filepath);
		virtual ~WindowsModule() override;

	protected:
		virtual FnPtr OpenFunction(const String& name) override;
	private:
		HMODULE m_ModuleHandle;
	};

}
