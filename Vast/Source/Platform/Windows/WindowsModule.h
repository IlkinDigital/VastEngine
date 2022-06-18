#pragma once

#include "Module/RuntimeModule.h"

namespace Vast {

	class WindowsModule : public RuntimeModule
	{
	public:
		WindowsModule(const Filepath& filepath);
		virtual ~WindowsModule() override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual void Clear() override;
	protected:
		virtual FnPtr OpenFunction(const String& name) override;
	private:
		bool m_IsLoaded = false;
		HMODULE m_ModuleHandle;
	};

}
