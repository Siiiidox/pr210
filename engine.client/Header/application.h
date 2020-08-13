#pragma once
#include "applicationWindow.h"
#include "d3d11renderer.h"
#include <chrono>
#include "camera.h"
#include "mesh.h"

class Application
{

private:
	ApplicationWindow window;
	Engine::Graphics::D3D11Renderer renderer;
	Camera camera;
	Engine::Resources::Cube cube;
	enum class ApplicationState
	{
		Started,
		Running,
		Stopped
	} currentState = ApplicationState::Started;
	std::chrono::steady_clock::time_point t_start = std::chrono::high_resolution_clock::now();
public:
	void Init();
	void Run();
	void Shutdown();

};