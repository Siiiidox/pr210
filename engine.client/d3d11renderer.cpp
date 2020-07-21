#include "d3d11renderer.h"
#include "util.h"
#include <iostream>
#include "vec3.h"
#include "vec2.h"
#include "color.h"
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
	VertexType vertices[4] = 
	{ 
		{ Vec3{-0.5f, -0.5f, 0.0f}, FloatColor{1, 0, 0, 1}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,1.f} },
		{ Vec3{0.5f, -0.5f, 0.0f}, FloatColor{1, 0, 0, 1}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{-0.5f, 0.5f, 0.0f}, FloatColor{1, 0, 0, 1}, Vec3{0.f, 0.f, -1.f}, Vec2{0.f,1.f}},
		{ Vec3{0.5f, 0.5f, 0.0f}, FloatColor{1, 0, 0, 1}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} }
	};
	int indices[6] =
	{
		0,2,1,
		1,2,3
	};

	//Create Vertex Buffer Description
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth= sizeof(vertices);
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	//Create Vertex Buffer Resource
	D3D11_SUBRESOURCE_DATA vbSubResource = {};
	vbSubResource.SysMemPitch = 0;
	vbSubResource.SysMemSlicePitch = 0;
	vbSubResource.pSysMem = &vertices;
	//Tell device to create Vertex Buffer
	if (FAILED(device->CreateBuffer(&vbDesc, &vbSubResource, &QuadVertexBuffer)))
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//Create Index Buffer Description
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	//Create Index Buffer Resource
	D3D11_SUBRESOURCE_DATA ibSubResource = {};
	ibSubResource.SysMemPitch = 0;
	ibSubResource.SysMemSlicePitch = 0;
	ibSubResource.pSysMem = &indices;
	//Tell device to create Index Buffer
	if (FAILED(device->CreateBuffer(&ibDesc, &ibSubResource, &QuadIndexBuffer)))
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
	//Retrieve 
	ui32 numModes;
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
	{
		MessageBoxA(NULL, "Could not retrieve display mode count", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	DXGI_MODE_DESC* modes = new DXGI_MODE_DESC[numModes];
	if (modes == nullptr)
	{
		MessageBoxA(NULL, "Could not allocate mode descriptions", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modes)))
	{
		MessageBoxA(NULL, "Could not retrieve display modes", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	ui32 width, height;
	if (!window.GetClientSize(width, height))
	{
		MessageBoxA(NULL, "Could not retrieve client application size", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	ui32 refreshNum = 0;
	ui32 refreshDenom = 0;
	for (ui32 i = 0; i < numModes; i++)
	{
		if (modes[i].Width == width && modes[i].Height == height)
		{
			refreshNum = modes[i].RefreshRate.Numerator;
			refreshDenom = modes[i].RefreshRate.Denominator;
		}
	}
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	if (FAILED(adapter->GetDesc1(&adapterDesc)))
	{
		MessageBoxA(NULL, "Could not retrieve adapter description", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	{
		char adapterDescText[128];
		wcstombs(adapterDescText, adapterDesc.Description, 128);
		printf("Graphics Device: %s\n", adapterDescText);
		printf("Graphics available Memory: %d\n", static_cast<ui32>(adapterDesc.DedicatedVideoMemory * 9.5367E-7f));
	}
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
	//Set format to requested mode format
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
	
	ID3D11Texture2D* surface = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&surface))))
	{
		MessageBoxA(NULL, "Could not get back-buffer interface", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC RTVdesc = {};
	if (FAILED(device->CreateRenderTargetView(surface, 0, &rtv)))
	{
		MessageBoxA(NULL, "Could not create render target view", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	SAFERELEASE(surface);
	ID3D10DepthStencilView * stencilView = nullptr;
	context->OMSetRenderTargets(1, &rtv, nullptr);
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
	return true;
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
}
