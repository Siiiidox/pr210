#pragma once
#include "applicationWindow.h"

class Application
{

private:
	ApplicationWindow window;
	enum class ApplicationState
	{
		Started,
		Running,
		Stopped
	} currentState;
public:
	void Init();
	void Run();
	void Shutdown();

};