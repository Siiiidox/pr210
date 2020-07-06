#pragma once
#include "AppWindow.h"
class Application
{
private:
	AppWindow window;
	enum class AppState
	{
		Started,
		Running,
		Stopped
	} appState;
public:
	void Init();
	void Run();
	void Shutdown();
};