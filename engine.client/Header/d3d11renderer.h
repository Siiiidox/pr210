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
	/*!
	*	@brief Struct for creating a MeshObject
	*	@TODO Only here to avoid errors. Corretly created in Mesh.h Needs to be adjust
	*/
	struct MeshObject;
}

namespace Engine::Graphics
{
	class D3D11Renderer
	{
	private:		
	    /*!
		*	@brief define a generic pointer to replace plattform specific pointer like for windows ID3D11Buffer*
		*/
		typedef int* GraphicsBufferPtr;
		/*!
		*	@brief should vsync be Enabled
		*/
		bool vsyncEnabled = false;
		/*!
		*	@brief variable to store the SwapChain for the D3D11Device
		*/
		IDXGISwapChain* swapChain = nullptr;
		/*!
		*	@brief variable to store the DeviceContext
		*/
		ID3D11DeviceContext* immediateContext = nullptr;
		/*!
		*	@brief variable to store the targetView
		*/
		ID3D11RenderTargetView* targetView = nullptr;
		/*!
		*	@brief a Color to clear the View. Will be displayed as Room/Background Color.
		*/
		FLOAT clearColor[4] = { 0.f, 0.1f, 0.2f, 1.f };

		/*!
		*	@brief Meshbuffer
		*/
		GraphicsBufferPtr transformBuffer = nullptr;

		//Shader
		/*!
		*	@brief variable to store the VertexShader
		*/
		ID3D11VertexShader* vertexShader = nullptr;
		/*!
		*	@brief variable to store the InputLayout for the VertexShader
		*/
		ID3D11InputLayout* vertexLayout = nullptr;
		/*!
		*	@brief variable to store the PixelShader
		*/
		ID3D11PixelShader* pixelShader = nullptr;
		/*!
		*	@brief varialbe to store the textureShader
		*/
		ID3D11PixelShader* textureShader = nullptr;

		//Camera
		/*!
		*	@brief constant variable to store the active Camera
		*/
		const Camera* activeCamera = nullptr;
		/*!
		*	@brief variable to store the Buffer for the camera
		*/
		GraphicsBufferPtr cameraBuffer = nullptr;
		/*!
		*	@brief variables to store the width and height of the ApplicationWindow
		*/
		ui32 width = 0, height = 0;

	
	public:
		/*!
		*	@brief variable to store the Device
		*/
		ID3D11Device* device = nullptr;
		/*!
		*	@brief Initalize ApplicationWindow.
		*	@param window required the ApplicationWindow.
		*	@return boolean if the intialization failed.
		*/
		bool Init(ApplicationWindow& window);
		/*!
		*	@brief beginning the Scene and clear the TargetView
		*	@return void
		*/
		void BeginScene();
		/*!
		*	@brief ending the Scene and Present the Swapchain
		*	@return void
		*/
		void EndScene();
		/*!
		*	@brief shutsdown the application and Saferelease the Variables
		*	@return void
		*/
		void Shutdown();
		/*!
		*	@brief Render a Quad
		*	@param required a MeshObject quad
		*	@return void
		*/
		void RenderQuad(Engine::Resources::MeshObject quad);
		/*!
		*	@brief Render a Cube
		*	@param required a MeshObject cube
		*	@return void
		*/
		void RenderCube(Engine::Math::Quaternion rotation, Engine::Resources::MeshObject cube);
		/*!
		*	@brief Creates the Shader and store them in the variables
		*	@return void
		*/
		void CreateShader();
		/*!
		*	@brief sets the provides camera as active
		*	@param required the camera which should be set active
		*	@return void
		*/
		void SetActiveCamera(const Camera& camera);
	};
}
