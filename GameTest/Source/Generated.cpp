#include "Generated.h"

#include "Vast/ApplicationCore/Application.h"

#include "CharacterController.h"

extern "C"
{
	SCRIPT_API void AddNativeScript(Vast::Entity entity)
	{
		entity.AddOrReplaceComponent<Vast::NativeScriptComponent>().Bind<Vast::CharacterController>();
	}

	SCRIPT_API void Init(Vast::Application* appPtr)
	{
		Vast::Application::Set(appPtr);
		Vast::Logger::Init();
		Vast::Application::Get().GetWindow().Init();
	}
}