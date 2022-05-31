#pragma once

#include "CoreMinimal.h"

namespace Vast {

	class RuntimeModule
	{
	public:
		virtual ~RuntimeModule() = default; 

		template<typename ReturnTy>
		ReturnTy LoadFunction(const String& name);

		static Ref<RuntimeModule> Create(const String& filepath);
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