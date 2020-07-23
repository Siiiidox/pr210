#include "d3d11renderer.h"
#include "util.h"
#include <iostream>
#include "vec3.h"
#include "vec2.h"
#include "mat4x4.h"
#include "color.h"
#include <d3dcompiler.h>
#include "vertex.h"

using namespace Engine::Math;
using namespace Engine::Utils;

GE_DEFINE_SINGLETON(Engine::Graphics::D3D11Renderer)

bool Engine::Graphics::D3D11Renderer::Init(Engine::Core::AppWindow& window)
{
	IDXGIFactory1 * factory = nullptr;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory))))
	{
		MessageBoxA(NULL, "Could not create DX factory", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//Adapter -> Interface for GPU
	IDXGIAdapter1* adapter = nullptr;
	if (FAILED(factory->EnumAdapters1(0, &adapter)))
	{
		MessageBoxA(NULL, "Could not enumerate adapter", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//Output -> Interface for Displays
	IDXGIOutput* output = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &output)))
	{
		MessageBoxA(NULL, "Could not enumerate outputs", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//Retrieve the amount of modes available to us with this output
	ui32 numModes;
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
	{
		MessageBoxA(NULL, "Could not retrieve display mode count", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//Allocate array for the modes
	DXGI_MODE_DESC* modes = new DXGI_MODE_DESC[numModes];
	if (modes == nullptr)
	{
		MessageBoxA(NULL, "Could not allocate mode descriptions", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//read all modes from the output
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modes)))
	{
		MessageBoxA(NULL, "Could not retrieve display modes", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	//Needs to be client size as we don't want to draw over the windows head and border
	if (!window.GetClientSize(width, height))
	{
		MessageBoxA(NULL, "Could not retrieve client application size", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	ui32 refreshNum = 0;
	ui32 refreshDenom = 0;
	//Get the mode that matches our client size don't break so we get the one with highest refresh rate
	for (ui32 i = 0; i < numModes; i++)
	{
		if (modes[i].Width == width && modes[i].Height == height)
		{
			refreshNum = modes[i].RefreshRate.Numerator;
			refreshDenom = modes[i].RefreshRate.Denominator;
		}
	}
	
	//Let us retrieve the description for our Graphics card
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	if (FAILED(adapter->GetDesc1(&adapterDesc)))
	{
		MessageBoxA(NULL, "Could not retrieve adapter description", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//Output the name and available memory to the console
	{
		char adapterDescText[128];
		wcstombs(adapterDescText, adapterDesc.Description, 128);
		printf("Graphics Device: %s\n", adapterDescText);
		printf("Graphics available Memory: %d MB\n", static_cast<ui32>(adapterDesc.DedicatedVideoMemory * 9.5367E-7f));
	}
	//Release and delete stuff we don't need anymore in next code section
	SAFEDELETEARR(modes);

	SAFERELEASE(output);
	SAFERELEASE(adapter);
	SAFERELEASE(factory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	//Double buffer
	swapChainDesc.BufferCount = 2;
	//Set Buffersize to client application size
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	//Set format to requested mode format --- needs to be the same as in GetDisplayModeList above
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	//Set Refreshrate as Rational if Vsync is enabled
	swapChainDesc.BufferDesc.RefreshRate.Numerator = (vsyncEnable ? refreshNum : 0);
	swapChainDesc.BufferDesc.RefreshRate.Denominator = (vsyncEnable && refreshDenom > 0 ? refreshDenom : 1);
	//We dont care how scanline is ordered or buffer is scaled
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//mark this buffer for output
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//assign our created window to it
	swapChainDesc.OutputWindow = reinterpret_cast<HWND>(window.GetHandle());
	// make lowest sampler
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	//our window was created windows
	swapChainDesc.Windowed = true;
	//Discard the when swap
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//No flags
	swapChainDesc.Flags = 0;

	//Feature level to request from D3D
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context)))
	{
		MessageBoxA(NULL, "Could not create device and swap chain", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	//Create a texture2D that will represent the screen image
	ID3D11Texture2D* surface = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&surface))))
	{
		MessageBoxA(NULL, "Could not get back-buffer interface", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//Create a render target view from the texture2D we just created
	D3D11_RENDER_TARGET_VIEW_DESC RTVdesc = {};
	if (FAILED(device->CreateRenderTargetView(surface, 0, &rtv)))
	{
		MessageBoxA(NULL, "Could not create render target view", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//Release the texture2D as we assigned it to the render target view
	SAFERELEASE(surface);


	//Create a Texture2D with 1 channel that the depth stencil buffer can write to
	ID3D11Texture2D* depthSurface = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, &depthSurface)))
	{
		MessageBoxA(NULL, "Could not create depth buffer texture", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//Create the depth stencil state that will descripe how depth is calculate if enabled
	D3D11_DEPTH_STENCIL_DESC stencilDesc = {};
	stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	stencilDesc.DepthEnable = true;
	stencilDesc.StencilEnable = false;
	ID3D11DepthStencilState* depthState = nullptr;
	if (FAILED(device->CreateDepthStencilState(&stencilDesc, &depthState)))
	{
		MessageBoxA(NULL, "Could not create depth buffer state", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	context->OMSetDepthStencilState(depthState, 1);

	//Create the depth view from the Texture
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ID3D11DepthStencilView* depthView = nullptr;
	if (FAILED(device->CreateDepthStencilView(depthSurface, &depthViewDesc, &depthView)))
	{
		MessageBoxA(NULL, "Could not create depth view", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//Set our rendertarget to the target view
	context->OMSetRenderTargets(1, &rtv, depthView);

	
	
	/*
	Create a rasterizer to tell the cpu what to cull and how the index winding is done
	Addionally a variable is addded to enable a wireframe mode just for fun
	*/
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.CullMode = wireframe ? D3D11_CULL_NONE : D3D11_CULL_BACK;
	rasterDesc.MultisampleEnable = wireframe ? false : true;
	rasterDesc.AntialiasedLineEnable = wireframe;
	rasterDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

	if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterState)))
	{
		MessageBoxA(NULL, "Could not create raster State", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	context->RSSetState(rasterState);
	//Set our viewport to the size of our client size
	D3D11_VIEWPORT viewport
	{
		0.0f,
		0.0f,
		static_cast<FLOAT>(width),
		static_cast<FLOAT>(height),
		0.0f,
		1.0f
	};
	context->RSSetViewports(1, &viewport);

	transformBuffer = CreateBuffer(BufferType::Constant, &Mat4x4::Zero, sizeof(Mat4x4), UsageType::Dynamic);
	if (!transformBuffer)
	{
		MessageBoxA(NULL, "Could not create transform buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	//GenerateQuad();
	CreateShader();
	return true;
}

void Engine::Graphics::D3D11Renderer::CreateShader()
{
	//Load compiled Vertex Shader file to a blob
	ID3DBlob* vertexShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"./data/shd/vertexDefault.shader", &vertexShaderBlob)))
	{
		MessageBoxA(NULL, "Could not load vertex shader", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Load compiled Pixel Shader file to a blob
	ID3DBlob* pixelShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"./data/shd/pixelDefault.shader", &pixelShaderBlob)))
	{
		MessageBoxA(NULL, "Could not load pixel shader", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Create a Vertex Shader from blob
	if (FAILED(device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)))
	{
		MessageBoxA(NULL, "Could not create pixel shader", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Create a Pixel Shader from blob
	if (FAILED(device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)))
	{
		MessageBoxA(NULL, "Could not create pixel shader", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Create Input Elements for the vertex Shader
	D3D11_INPUT_ELEMENT_DESC vertexElemDesc[4] = { };

	/*
	D3D11_INPUT_PER_VERTEX_DATA to declare that these elements are for a vertex shader
	D3D11_APPEND_ALIGNED_ELEMENT to append the element next to each other at the end of each data segment
	SemanticIndex -> declare which of the shematic with the name SemanticName is used e.g.
	SemanticIndex = 0
	SemanticName = "POSITION"
	 -> POSITOON0
	AlignedByteOffset if the data need to be aligned with a specific offset
	*/
	//Format for 3 Float for the position vec3
	vertexElemDesc[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	vertexElemDesc[0].InputSlot = 0;
	vertexElemDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElemDesc[0].AlignedByteOffset = 0;
	vertexElemDesc[0].SemanticIndex = 0;
	vertexElemDesc[0].SemanticName = "POSITION";
	//Format for 4 Float for the color r g b a
	vertexElemDesc[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertexElemDesc[1].InputSlot = 0;
	vertexElemDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElemDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElemDesc[1].SemanticIndex = 0;
	vertexElemDesc[1].SemanticName = "COLOR";
	//Format for 3 Float for the normal vec3
	vertexElemDesc[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	vertexElemDesc[2].InputSlot = 0;
	vertexElemDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElemDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElemDesc[2].SemanticIndex = 0;
	vertexElemDesc[2].SemanticName = "NORMAL";
	//Format for 2 Float for the texcoord/uv vec2
	vertexElemDesc[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	vertexElemDesc[3].InputSlot = 0;
	vertexElemDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElemDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElemDesc[3].SemanticIndex = 0;
	vertexElemDesc[3].SemanticName = "TEXCOORD";

	//Create a Input Layout for the vertex shader with the descriptions and count of the elements using the data read into the blob
	if (FAILED(device->CreateInputLayout(vertexElemDesc, sizeof(vertexElemDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &vertexLayout)))
	{
		MessageBoxA(NULL, "Could not create vertex input layout", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//No need to create Input Elements for the pixel shader as the vertex shader give data to it in the shader code
}

void Engine::Graphics::D3D11Renderer::BeginScene()
{
	context->ClearRenderTargetView(rtv, clearColor);

	if (activeCamera)
	{
		//Map the contents of the cameraBuffer in CPU memory to be accessed by CPU
		D3D11_MAPPED_SUBRESOURCE camResource = {};
		if (FAILED(context->Map(reinterpret_cast<ID3D11Resource*>(cameraBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &camResource)))
		{
			MessageBoxA(NULL, "could not map transform buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		//Convert mapped data to desired type
		Mat4x4* dataMat = reinterpret_cast<Mat4x4*>(camResource.pData);

		//Calculate our projection from the camera data
		Mat4x4 viewProjMat = Mat4x4::Identity;
		if (activeCamera->cameraType == Camera::CameraType::Perspective)
		{
			viewProjMat = Mat4x4::FromPerspectiveFOV(activeCamera->h_fov, static_cast<real>(width) / static_cast<real>(height), activeCamera->near_plane, activeCamera->far_plane) * Mat4x4::FromView(activeCamera->transform.position);
		}
		else
		{
			viewProjMat = Mat4x4::FromOrthographic(static_cast<real>(width), static_cast<real>(height), activeCamera->near_plane, activeCamera->far_plane) * Mat4x4::FromView(activeCamera->transform.position);
		}
		//Assign new data to be uploaded
		if (dataMat)(*dataMat) = viewProjMat;
		//Unmap to confirm upload and discard old data
		context->Unmap(reinterpret_cast<ID3D11Resource*>(cameraBuffer), 0);
	}
}

void Engine::Graphics::D3D11Renderer::EndScene()
{
	if (vsyncEnable)
		swapChain->Present(1, 0);
	else
		swapChain->Present(0, 0);
	
}

void Engine::Graphics::D3D11Renderer::Shutdown()
{
	//exit fullscreen mode if we are in it
	if (swapChain)
		swapChain->SetFullscreenState(false, nullptr);

	//Delete / Release in order of creation
	SAFERELEASE(rtv);
	SAFERELEASE(context);
	SAFERELEASE(device);
	SAFERELEASE(swapChain);

	SAFERELEASE(vertexLayout);
	SAFERELEASE(vertexShader);
	SAFERELEASE(pixelShader);
}

void Engine::Graphics::D3D11Renderer::RenderObject(Transform transform, int indexCount, GraphicsBufferPtr vertexBuffer, GraphicsBufferPtr indexBuffer)
{
	//INPUT ASSEMBLER STAGE

	ui32 stride = sizeof(Vertex);
	ui32 offset = 0;

	//Generate model Matrix from Position/Translation Rotation Scale -> TRS
	Mat4x4 modelMat = Mat4x4::FromTranslation(transform.position) * Mat4x4::FromOrientation(transform.rotation) * Mat4x4::FromScale(transform.scale);

	//Map the contents of the transformBuffer in CPU memory to be accessed by CPU
	D3D11_MAPPED_SUBRESOURCE modelResource = {};
	if (FAILED(context->Map(reinterpret_cast<ID3D11Resource*>(transformBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &modelResource)))
	{
		MessageBoxA(NULL, "could not map transform buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Convert mapped data to desired type
	Mat4x4* dataMat = reinterpret_cast<Mat4x4*>(modelResource.pData);
	//Assign new data to be uploaded
	if(dataMat)(*dataMat) = modelMat;

	//Unmap to confirm upload and discard old data
	context->Unmap(reinterpret_cast<ID3D11Resource*>(transformBuffer), 0);

	//Bind the transformation for rendering this object
	context->VSSetConstantBuffers(0, 1, reinterpret_cast<ID3D11Buffer**>(&transformBuffer));
	//Bind the camera transformation for rendering this object
	context->VSSetConstantBuffers(1, 1, reinterpret_cast<ID3D11Buffer**>(&cameraBuffer));
	//Bind the vertex data that describes the object we want to render
	context->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D11Buffer**>(&vertexBuffer), &stride, &offset);
	//Bind the index data that describes the faces of the object we want to render
	context->IASetIndexBuffer(reinterpret_cast<ID3D11Buffer*>(indexBuffer), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//The the assembler what type of data he will be working with
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Tell the assembler how the vertex shader inputs look like
	context->IASetInputLayout(vertexLayout);

	//VERTEX SHADER STAGE

	context->VSSetShader(vertexShader, nullptr, 0);

	//PIXEL SHADER STAGE

	context->PSSetShader(pixelShader, nullptr, 0);

	//OUTPUT MERGER STAGE

	context->OMSetRenderTargets(1, &rtv, nullptr);

	//Draw the object with the amount indices the object has
	context->DrawIndexed(indexCount, 0, 0);
}

void Engine::Graphics::D3D11Renderer::SetActiveCamera(const Camera& camera)
{
	if (!activeCamera)
	{
		activeCamera = &camera;

		//Create a buffer with the calculated matrix of the camera
		Mat4x4 viewProjMat = Mat4x4::Identity;
		if (activeCamera->cameraType == Camera::CameraType::Perspective)
		{
			viewProjMat = Mat4x4::FromPerspectiveFOV(activeCamera->h_fov, static_cast<real>(width) / static_cast<real>(height), activeCamera->near_plane, activeCamera->far_plane) * Mat4x4::FromView(activeCamera->transform.position);
		}
		else
		{
			viewProjMat = Mat4x4::FromOrthographic(static_cast<real>(width), static_cast<real>(height), activeCamera->near_plane, activeCamera->far_plane) * Mat4x4::FromView(activeCamera->transform.position);
		}
		cameraBuffer = CreateBuffer(BufferType::Constant, &viewProjMat, sizeof(Mat4x4), UsageType::Dynamic);
		if (!cameraBuffer)
		{
			MessageBoxA(NULL, "Could not set camera", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		}
	}
}

GraphicsBufferPtr Engine::Graphics::D3D11Renderer::CreateBuffer(BufferType type, const void* data, int dataSize, UsageType usage)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	switch (usage)
	{
	case UsageType::Dynamic:
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		break;
	default:
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		break;
	}
	bufferDesc.ByteWidth = dataSize;

	switch (type)
	{
	case BufferType::Vertex:
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		break;
	case BufferType::Index:
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		break;
	case BufferType::Constant:
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		break;
	default:
		MessageBoxA(NULL, "invalid buffer type", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return nullptr;
	}
	if(type == BufferType::Constant)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	/*
	Create Vertex Buffer Resource
	pSysMem the pointer to the data to be uploaded to the buffer
	*/
	D3D11_SUBRESOURCE_DATA bufferSubResource = {};
	bufferSubResource.pSysMem = data;
	bufferSubResource.SysMemPitch = 0;
	bufferSubResource.SysMemSlicePitch = 0;
	ID3D11Buffer* buffer = nullptr;
	//Tell device to create Vertex Buffer and the data in subresource
	if (FAILED(device->CreateBuffer(&bufferDesc, &bufferSubResource, &buffer)))
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return nullptr;
	}
	return reinterpret_cast<GraphicsBufferPtr>(buffer);
}
