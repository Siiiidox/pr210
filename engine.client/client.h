#pragma once

// EXTERNAL INCLUDES
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
	public:
		void Init();
		void Run();
		void Shutdown();
	};
}
