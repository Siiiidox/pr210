#pragma once

// EXTERNAL INCLUDES
#include <chrono>
// INTERNAL INCLUDES
#include "appwindow.h"
#include "camera.h"
#include "mesh.h"
namespace Game::Client
{
	class Application
	{
	private:
		Engine::Core::AppWindow window;
		Camera camera;
		Engine::Resources::Mesh testMesh;
		enum class AppState
		{
			Started,
			Running,
			Stopped
		} appState = AppState::Started;
		std::chrono::steady_clock::time_point t_start = std::chrono::high_resolution_clock::now();
	public:
		void Init();
		void Run();
		void Shutdown();
	};
}
