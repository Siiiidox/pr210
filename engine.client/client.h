#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "appwindow.h"
#include "d3d11renderer.h"
namespace Game::Client
{
	class Application
	{
	private:
		Engine::Graphics::D3D11Renderer renderer;
		Engine::Core::AppWindow window;
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
