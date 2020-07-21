#pragma once
#include "applicationWindow.h"
#include "d3d11renderer.h"

class Application
{

private:
	ApplicationWindow window;
	Engine::Graphics::D3D11Renderer renderer;
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