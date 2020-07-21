#include "d3d11renderer.h"
#include "util.h"
#include <iostream>
#include "vec3.h"
#include "vec2.h"
#include "color.h"
#include <d3dcompiler.h>
using namespace Engine::Math;
using namespace Engine::Utils;

struct VertexType
{
	Vec3 position;
	FloatColor color;
	Vec3 normal;
	Vec2 texCoords;
};

void Engine::Graphics::D3D11Renderer::GenerateQuad()
{
	//Create the verices required to represent a quad
	VertexType vertices[4] = 
	{ 
		{ Vec3{-0.5f, -0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,1.f} },
		{ Vec3{0.5f, -0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{-0.5f, 0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{0.f,1.f}},
		{ Vec3{0.5f, 0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} }
	};
	//index the verices counterclock wise
	//Counterclock wise because D3D uses it by default
	int indices[6] =
	{
		0,2,1,
		1,2,3
	};

	/*
	Create Vertex Buffer Description
	D3D11_BIND_VERTEX_BUFFER to describe this buffer as vertex buffer
	CPUAccessFlags to define if cpu and/or gpu can write to or read from it
	ByteWidth the size/count of the data/vertices
	MiscFlags to mark it in special ways
	*/
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(vertices);
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	/*
	Create Vertex Buffer Resource
	pSysMem the pointer to the data to be uploaded to the buffer
	*/
	D3D11_SUBRESOURCE_DATA vbSubResource = {};
	vbSubResource.pSysMem = &vertices;
	vbSubResource.SysMemPitch = 0;
	vbSubResource.SysMemSlicePitch = 0;
	//Tell device to create Vertex Buffer and the data in subresource
	if (FAILED(device->CreateBuffer(&vbDesc, &vbSubResource, &quadVertexBuffer)))
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	/*
	Create Index Buffer Description
	D3D11_BIND_VERTEX_BUFFER to describe this buffer as index buffer
	CPUAccessFlags to define if cpu and/or gpu can write to or read from it
	ByteWidth the size/count of the data/indices
	MiscFlags to mark it in special ways
	*/
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	/*
	Create Index Buffer Resource
	pSysMem the pointer to the data to be uploaded to the buffer
	*/
	D3D11_SUBRESOURCE_DATA ibSubResource = {};
	ibSubResource.pSysMem = &indices;
	ibSubResource.SysMemPitch = 0;
	ibSubResource.SysMemSlicePitch = 0;
	//Tell device to create Index Buffer and the data in subresource
	if (FAILED(device->CreateBuffer(&ibDesc, &ibSubResource, &quadIndexBuffer)))
	{
		MessageBoxA(NULL, "Could not create index buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
}

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
	ui32 width, height;
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

	ID3D10DepthStencilView * stencilView = nullptr;
	//Set our rendertarget to the target view
	context->OMSetRenderTargets(1, &rtv, nullptr);
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

	GenerateQuad();
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
	SAFERELEASE(quadVertexBuffer);
	SAFERELEASE(quadIndexBuffer);
}

void Engine::Graphics::D3D11Renderer::RenderQuad()
{

	//INPUT ASSEMBLER STAGE

	ui32 stride = sizeof(VertexType);
	ui32 offset = 0;

	context->IASetVertexBuffers(0, 1, &quadVertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(vertexLayout);

	//VERTEX SHADER STAGE

	context->VSSetShader(vertexShader, nullptr, 0);

	//PIXEL SHADER STAGE

	context->PSSetShader(pixelShader, nullptr, 0);

	//OUTPUT MERGER STAGE

	context->OMSetRenderTargets(1, &rtv, nullptr);

	context->DrawIndexed(6, 0, 0);
}
