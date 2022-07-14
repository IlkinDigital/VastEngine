#pragma once

#include "Application.h"

#include <optick.h>

extern Vast::Application* Vast::CreateApplication();

int main(int argc, char** argv)
{
	OPTICK_APP("VastEngine");

	Vast::Logger::Init();

	Vast::Application* app = Vast::CreateApplication();

	app->Get().GetWindow().SetVSync(true);

	app->Run();

	OPTICK_SHUTDOWN();

	delete app;

	VAST_CORE_INFO("Not freed total: {0}", s_Tracker.CurrentUsage());
}