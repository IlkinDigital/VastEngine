#include "Vast/ApplicationCore/EntryPoint.h"

#include "MudboxLayer.h"

class MudboxApp : public Vast::Application
{
public:
	MudboxApp(const Vast::String& name)
		: Application(name)
	{
		PushLayer(new Vast::MudboxLayer("MudboxApp"));
	}

	~MudboxApp() override
	{
	}
};

Vast::Application* Vast::CreateApplication()
{
	return new MudboxApp("Mudbox");
}
