// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "client.h"
#include "types.h"

using namespace Game::Client;

void Application::Init()
{
	AnsiString name = "PR210 Engine";
	window.Init(name, 1280, 720);
	window.Show();
	if (!renderer.Init(window))
	{
		return;
	}
	this->appState = AppState::Running;
}

void Application::Run()
{
	while(this->appState == AppState::Running)
	{
		renderer.BeginScene();
		renderer.RenderQuad();
		renderer.EndScene();
		if (!window.MessagePump())
		{
			this->appState = AppState::Stopped;
		}
		
	}
}

void Application::Shutdown()
{
	renderer.Shutdown();
}
