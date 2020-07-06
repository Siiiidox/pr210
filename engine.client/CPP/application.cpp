#include "application.h"
#include "types.h"

void Application::Init()
{

	window.Init("PRO210 Engine", 1280, 720);
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

	}

}
void Application::Shutdown()
{

}

