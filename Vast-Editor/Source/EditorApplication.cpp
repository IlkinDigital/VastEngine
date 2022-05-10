#include <Vast.h>

class EditorApp : public Vast::Application
{
public:
	EditorApp(const Vast::String& name)
		: Application(name)
	{
		VAST_TRACE("Hello");
	}

	~EditorApp() override
	{
		VAST_TRACE("Bye");
	}
};

Vast::Application* Vast::CreateApplication()
{
	return new EditorApp("Vast Editor");
}