#pragma once

#include <dxgi.h>
#include <d3d11.h>

#include "types.h"
#include "applicationWindow.h"

namespace Engine::Graphics
{

	class D3D11Renderer
	{
	private:
		bool vsyncEnabled = false;
		IDXGISwapChain* swapChain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* immediateContext = nullptr;
		ID3D11RenderTargetView* targetView = nullptr;
		FLOAT clearColor[4] = { 0.f, 0.8f, 1.f, 1.f };

		ID3D11Buffer* quadVertexBuffer = nullptr;
		ID3D11Buffer* quadIndexBuffer = nullptr;
		//Shader
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11InputLayout* vertexLayout = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		void GenerateQuad();
	public:
		bool Init(ApplicationWindow& window);
		void BeginScene();
		void EndScene();
		void Shutdown();

		void RenderQuad();
		void CreateShader();
	};
}
