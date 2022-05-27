#include "SandboxLayer.h"

class SandboxApp : public Application
{
public:
	SandboxApp(const String& name)
		: Application(name)
	{
		PushLayer(new SandboxLayer("Sandbox"));
	}

	~SandboxApp() override
	{
	}
};

Vast::Application* Vast::CreateApplication()
{
	return new SandboxApp("Sandbox");
}