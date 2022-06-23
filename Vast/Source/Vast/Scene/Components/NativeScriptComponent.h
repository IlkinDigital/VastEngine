#pragma once

#include "Scene/ScriptableEntity.h"

namespace Vast {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		String Name = "None";
		bool IsBound = false;

		std::function<ScriptableEntity* ()> InstantiateScript;
		std::function<void (NativeScriptComponent*)> DestroyScript;

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;

		template<typename Ty, typename... Args>
		void Bind(Args&&... args)
		{
			InstantiateScript = std::move([&]() 
				{ 
					return static_cast<ScriptableEntity*>(new Ty(std::forward<Args>(args)...));
				});

			DestroyScript = std::move([&](NativeScriptComponent* nsc) 
				{ 
					delete nsc->Instance; 
					nsc->Instance = nullptr; 
				});
			IsBound = true;
		}
	};

}