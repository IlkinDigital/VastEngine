#include "Vast/Scene/Entity.h"
#include "Vast/ApplicationCore/Application.h"

#define SCRIPT_API __declspec(dllexport)

extern "C"
{
	SCRIPT_API void AddNativeScript(Vast::Entity entity);
	SCRIPT_API void Init(Vast::Application* appPtr);
}
