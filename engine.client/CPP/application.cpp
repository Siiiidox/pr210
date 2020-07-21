#include "application.h"
#include "types.h"


void Application::Init()
{
	window.Init("PRO210 Engine", 1280, 720);
	if(!renderer.Init(window))
	{
		return; 
	}	
	this->currentState = ApplicationState::Running;
}

void Application::Run()
{

	while (this->currentState == ApplicationState::Running)
	{
		if (!window.MessagePump())
		{
			this->currentState = ApplicationState::Stopped;
		}
		renderer.BeginScene();





		renderer.EndScene();
	}

}
void Application::Shutdown()
{
	renderer.Shutdown();
}

