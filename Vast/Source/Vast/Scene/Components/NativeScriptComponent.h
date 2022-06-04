#pragma once

#include "Scene/ScriptableEntity.h"

namespace Vast {

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		String Name = "None";

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		NativeScriptComponent() = default;

		template<typename Ty>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new Ty()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}