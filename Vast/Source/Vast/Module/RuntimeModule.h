#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class RuntimeModule
	{
	public:
		virtual ~RuntimeModule() = default; 

		virtual void Clean() = 0;

		virtual bool IsLoaded() const = 0;

		template<typename ReturnTy>
		ReturnTy LoadFunction(const String& name);

		static Ref<RuntimeModule> Create(const Filepath& filepath);
	protected:
		typedef int64(*FnPtr)(void);
		virtual FnPtr OpenFunction(const String& name) = 0;
	};

	template<typename ReturnTy>
	inline ReturnTy RuntimeModule::LoadFunction(const String& name)
	{
		return (ReturnTy)OpenFunction(name);
	}
}