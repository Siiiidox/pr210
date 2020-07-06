#include "client.h"
#include "types.h"
void Application::Init()
{
	AnsiString name = "PR210 Engine";
	window.Init(name, 1280, 720);
	window.Show();
}

void Application::Run()
{
	this->appState = AppState::Running;
	while(this->appState == AppState::Running)
	{
		if (!window.MessagePump())
		{
			this->appState = AppState::Stopped;
		}
		
	}
}

void Application::Shutdown()
{

}