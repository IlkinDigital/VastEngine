#pragma once

#include "Module/RuntimeModule.h"

namespace Vast {

	class WindowsModule : public RuntimeModule
	{
	public:
		WindowsModule(const Filepath& filepath);
		virtual ~WindowsModule() override;

		virtual void Clean() override;
	protected:
		virtual FnPtr OpenFunction(const String& name) override;
	private:
		HMODULE m_ModuleHandle;
	};

}
