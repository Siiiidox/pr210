#include <iostream>

#include "d3d11renderer.h"
#include "utils.h"


bool Engine::Graphics::D3D11Renderer::Init(ApplicationWindow& window)
{
	IDXGIFactory1* factory = nullptr;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory))))
	{
		printf("Caught Error: CreateDXGIFactory1() FAILED!\n");
		return false;
	}

	// Adapter ist die Softwareimplementierung zur Grafikkarte.
	IDXGIAdapter1* adapter = nullptr;
	if (FAILED(factory->EnumAdapters1(0, &adapter)))
	{
		printf("Caught Error: EnumAdapters1() FAILED!\n");
		return false;
	}

	//Output sind die verfügbaren Monitorinterfaces.
	IDXGIOutput* output = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &output)))
	{
		printf("Caught Error: EnumOutputs() FAILED!\n");
		return false;
	}

	ui32 numModes = 0;
	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
	{
		printf("Caught Error: GetDisplayModeList(nullptr) FAILED!\n");
		return false;
	}

	DXGI_MODE_DESC* modeDesc = new DXGI_MODE_DESC[numModes];
	if (modeDesc == nullptr)
	{
		printf("Caught Error: modeDesc == nullptr FAILED!\n");
		return false;
	}

	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modeDesc)))
	{
		printf("Caught Error: GetDisplayModeList(modeDesc) FAILED!\n");
		return false;
	}

	ui32 refreshNumerator = 0;
	ui32 refreshDenominator = 0;
	ui32 width = 0;
	ui32 height = 0;
	for (ui32 i = 0; i < numModes; i++)
	{
		if (window.GetClientSize(width, height) && modeDesc[i].Width == width && modeDesc[i].Height == height)
		{
			refreshNumerator = modeDesc[i].RefreshRate.Numerator;
			refreshDenominator = modeDesc[i].RefreshRate.Denominator;
		}
	}
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	if (FAILED(adapter->GetDesc1(&adapterDesc)))
	{
		printf("Caught Error: GetDesc1() FAILED!\n");
		return false;
	}
	{
		char videoCardDescription[128];
		wcstombs(videoCardDescription, adapterDesc.Description, 128);
		printf("Graphics Device: %s\n", videoCardDescription);

		printf("Dedicated VideoMemory: %i\n", static_cast<ui32>(adapterDesc.DedicatedVideoMemory * 9.5367e-7f));
	}

	SAFEDELETEARR(modeDesc);

	SAFERELEASE(output);
	SAFERELEASE(adapter);
	SAFERELEASE(factory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = (vsyncEnabled ? refreshNumerator : 0);
	swapChainDesc.BufferDesc.RefreshRate.Denominator = (vsyncEnabled && refreshDenominator > 0 ? refreshDenominator : 1);
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = reinterpret_cast<HWND>(window.GetHandle());
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;


	D3D_FEATURE_LEVEL featueLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(0
		, D3D_DRIVER_TYPE_HARDWARE
		, 0
		, D3D11_CREATE_DEVICE_DEBUG
		, &featueLevel
		, 1
		, D3D11_SDK_VERSION
		, &swapChainDesc
		, &swapChain
		, &device
		, nullptr
		, &immediateContext
	)))
	{
		printf("Caught Error: D3D11CreateDeviceAndSwapChain failed!\n");
		return false;
	}
	ID3D11Texture2D* surface = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&surface))))
	{
		printf("Caught Error: swapChain->GetBuffer() failed!");
		return false;
	}

	if (FAILED(device->CreateRenderTargetView(surface, 0, &targetView)))
	{
		printf("Caught Error: device->CreateRenderTargetView() failed!\n");
		return false;
	}

	SAFERELEASE(surface);

	immediateContext->OMSetRenderTargets(1, &targetView, nullptr);
	D3D11_VIEWPORT viewport
	{
		0.f,
		0.f,
		static_cast<FLOAT>(width),
		static_cast<FLOAT>(height),
		0.f,
		1.f
	};
	immediateContext->RSSetViewports(1, &viewport);
	return true;
}

void Engine::Graphics::D3D11Renderer::Shutdown()
{
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}

	SAFERELEASE(targetView); 
	SAFERELEASE(immediateContext);
	SAFERELEASE(device);
	SAFERELEASE(swapChain);
}

void Engine::Graphics::D3D11Renderer::BeginScene()
{
	immediateContext->ClearRenderTargetView(targetView, clearColor);
}

void Engine::Graphics::D3D11Renderer::EndScene()
{
	if (vsyncEnabled)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}
}
