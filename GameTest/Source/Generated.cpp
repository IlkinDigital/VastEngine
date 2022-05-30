#include "Generated.h"

extern "C"
{
	__declspec(dllexport) void AddNativeScript(Vast::Entity entity)
	{
		entity.AddComponent<Vast::NativeScriptComponent>().Bind<Vast::CharacterController>();
	}
}