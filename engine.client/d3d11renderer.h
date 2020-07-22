#pragma once
// EXTERNAL INCLUDES
#include <dxgi.h>
#include <d3d11.h>
// INTERNAL INCLUDES
#include "types.h"
#include "appwindow.h"
#include "singleton.h"
#include "camera.h"
typedef int* GraphicsBufferPtr;

namespace Engine::Graphics
{
	class D3D11Renderer
	{
		GE_DECLARE_SINGLETON(D3D11Renderer)
	private:
		bool vsyncEnable = true;
		bool wireframe = false;

		IDXGISwapChain* swapChain = nullptr;
		//the render device
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;

		ID3D11RasterizerState* rasterState = nullptr;
		//The view to render to
		ID3D11RenderTargetView* rtv = nullptr;

		FLOAT clearColor[4] = { 0.529411f, 0.807843f, 0.921686f, 1.0f };
		//Mesh Buffers
		GraphicsBufferPtr transformBuffer = nullptr;
		ID3D11Buffer* quadVertexBuffer = nullptr;
		ID3D11Buffer* quadIndexBuffer = nullptr;
		//Shaders
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11InputLayout* vertexLayout = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		//Camera
		const Camera* activeCamera = nullptr;
		GraphicsBufferPtr cameraBuffer = nullptr;
		ui32 width = 0, height = 0;
		/// <summary>
		/// Generate Vertex and Index Buffer for a quad
		/// </summary>
		void GenerateQuad();
	public:
		enum class BufferType
		{
			Index,
			Vertex,
			Constant
		};
		enum class UsageType
		{
			Default,
			Dynamic
		};
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

		void RenderObject(Engine::Math::Transform transform, int indexCount, GraphicsBufferPtr vertexBuffer, GraphicsBufferPtr indexBuffer);

		void SetActiveCamera(const Camera& camera);
		GraphicsBufferPtr CreateBuffer(BufferType type, const void* data, int dataSize, UsageType usage = UsageType::Default);
		
	};
}