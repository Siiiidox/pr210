#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "appwindow.h"

namespace Game::Client
{
	class Application
	{
	private:
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
