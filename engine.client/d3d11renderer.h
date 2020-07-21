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
	private:
		bool vsyncEnable = false;
		IDXGISwapChain* swapChain = nullptr;
		//the render device
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		//The view to render to
		ID3D11RenderTargetView* rtv = nullptr;

		FLOAT clearColor[4] = { 0.529411f, 0.807843f, 0.921686f, 1.0f };
	public:
		bool Init(Engine::Core::AppWindow &window);
		void BeginScene();
		void EndScene();
		void Shutdown();
	};
}