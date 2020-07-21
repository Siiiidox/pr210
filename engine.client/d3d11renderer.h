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
		//Buffers
		ID3D11Buffer* quadVertexBuffer = nullptr;
		ID3D11Buffer* quadIndexBuffer = nullptr;
		//Shaders
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11InputLayout* vertexLayout = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		/// <summary>
		/// Generate Vertex and Index Buffer for a quad
		/// </summary>
		void GenerateQuad();
	public:
		bool Init(Engine::Core::AppWindow &window);
		/// <summary>
		/// Creates shaders and layout for vertex and pixel shader loaded from file
		/// </summary>
		void CreateShader();
		/// <summary>
		/// To begin a render and clear the screen with clearColor
		/// </summary>
		void BeginScene();
		/// <summary>
		/// To end a render and present rtv using vsync if enabled
		/// </summary>
		void EndScene();
		/// <summary>
		/// Release and clear/delete previously created pointers and exit fullscreen if required
		/// </summary>
		void Shutdown();
		/// <summary>
		/// Render the generated quad with loaded shaders
		/// </summary>
		void RenderQuad();
	};
}