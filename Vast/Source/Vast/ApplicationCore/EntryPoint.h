#pragma once

#include "Application.h"

extern Vast::Application* Vast::CreateApplication();

int main(int argc, char** argv)
{
	Vast::Logger::Init();

	Vast::Application* app = Vast::CreateApplication();

	app->Get().GetWindow().SetVSync(false);

	app->Run();

	delete app;
}