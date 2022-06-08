#pragma once

#include "Scene/ScriptableEntity.h"

namespace Vast {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		String Name = "None";
		bool IsBound = false;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;

		template<typename Ty>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new Ty()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			IsBound = true;
		}
	};

}