#pragma once

#include <dxgi.h>
#include <d3d11.h>

#include "types.h"
#include "applicationWindow.h"
#include "transform.h"
#include "mesh.h"
#include "camera.h"


namespace Engine::Resources
{
	struct Cube;
}

namespace Engine::Graphics
{
	class D3D11Renderer
	{
	private:
		//Define a generic pointer to replace plattform specific pointer like for windows ID3D11Buffer*
		typedef int* GraphicsBufferPtr;
		bool vsyncEnabled = false;
		IDXGISwapChain* swapChain = nullptr;
		ID3D11DeviceContext* immediateContext = nullptr;
		ID3D11RenderTargetView* targetView = nullptr;
		FLOAT clearColor[4] = { 0.f, 0.8f, 1.f, 1.f };
		//Mesh Buffers
		GraphicsBufferPtr transformBuffer = nullptr;
		//Shader
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11InputLayout* vertexLayout = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;

		//Camera
		const Camera* activeCamera = nullptr;
		GraphicsBufferPtr cameraBuffer = nullptr;
		ui32 width = 0, height = 0;
	public:
		ID3D11Device* device = nullptr;
		/*!
		*	@brief Initalize ApplicationWindow.
		*	@param window required the ApplicationWindow.
		*/
		bool Init(ApplicationWindow& window);
		void BeginScene();
		void EndScene();
		void Shutdown();

		void RenderCube(Engine::Math::Quaternion rotation, Engine::Resources::Cube cube);
		void CreateShader();
		void SetActiveCamera(const Camera& camera);
	};
}
