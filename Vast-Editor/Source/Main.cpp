#include <Vast.h>
#include <Vast/ApplicationCore/Window.h>

int main()
{
	Vast::Logger::Init();
	Vast::WindowProps p("hi");
	VAST_TRACE("Hello Vast from application");
}