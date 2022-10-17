#include "Vast/ApplicationCore/EntryPoint.h"

#include "EditorLayer.h"
#include "DebugLayer.h"

class EditorApp : public Vast::Application
{
public:
	EditorApp(const Vast::String& name)
		: Application(name)
	{
		PushLayer(new Vast::EditorLayer("Vast-Editor"));
		PushLayer(new Vast::DebugLayer("Debug Layer"));
	}

	~EditorApp() override
	{
	}
};

Vast::Application* Vast::CreateApplication()
{
	return new EditorApp("Vast Editor");
}
