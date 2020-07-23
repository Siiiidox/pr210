#pragma once
// EXTERNAL INCLUDES
#include <dxgi.h>
#include <d3d11.h>
// INTERNAL INCLUDES
#include "types.h"
#include "appwindow.h"
#include "singleton.h"
#include "camera.h"

//Define a generic pointer to replace plattform specific pointer like for windows ID3D11Buffer*
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
		//Shaders
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11InputLayout* vertexLayout = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		//Camera
		const Camera* activeCamera = nullptr;
		GraphicsBufferPtr cameraBuffer = nullptr;
		ui32 width = 0, height = 0;

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
		/// <summary>
		/// 
		/// </summary>
		/// <param name="window">Window to init to</param>
		/// <returns>init was successful</returns>
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

		void RenderObject(Engine::Math::Transform transform, int indexCount, GraphicsBufferPtr vertexBuffer, GraphicsBufferPtr indexBuffer);
		/// <summary>
		/// Assign a camera to be used for rendering
		/// </summary>
		/// <param name="camera"></param>
		void SetActiveCamera(const Camera& camera);

		/// <summary>
		/// Create a Buffer in a specific type with data and the defined usage
		/// </summary>
		/// <param name="type"></param>
		/// <param name="data"></param>
		/// <param name="dataSize"></param>
		/// <param name="usage"></param>
		/// <returns>A generic pointer to be used for rendering or updating the buffer</returns>
		GraphicsBufferPtr CreateBuffer(BufferType type, const void* data, int dataSize, UsageType usage = UsageType::Default);
		
	};
}