#pragma once

#include "Application.h"

#include <optick.h>

extern Vast::Application* Vast::CreateApplication();

int main(int argc, char** argv)
{
	OPTICK_APP("VastEngine");

	Vast::Logger::Init();

	Vast::Application* app = Vast::CreateApplication();

	app->Get().GetWindow().SetVSync(false);

	app->Run();

	OPTICK_SHUTDOWN();

	delete app;
}