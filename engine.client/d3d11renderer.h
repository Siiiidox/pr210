#pragma once
// EXTERNAL INCLUDES
#include <dxgi.h>
#include <d3d11.h>
// INTERNAL INCLUDES
#include "types.h"
#include "appwindow.h"
namespace Engine::Graphics
{
	class D3D11Renderer
	{

	public:
		bool Init(Engine::Core::AppWindow &window);
		void Run();
		void Shutdown();
	};
}