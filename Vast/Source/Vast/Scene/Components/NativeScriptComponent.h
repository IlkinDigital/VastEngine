#pragma once

#include "Scripting/NativeScript.h"

namespace Vast {

	struct NativeScriptComponent
	{
		NativeScript* Instance = nullptr;
		String Name = "None";
		bool IsBound = false;

		std::function<NativeScript*()> InstantiateScript;

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;

		template<typename Ty, typename... Args>
		void Bind(Args&&... args)
		{
			InstantiateScript = [args...]()
				{ 
					return static_cast<NativeScript*>(new Ty(args...));
				};

			IsBound = true;
		}
	};

}