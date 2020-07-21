#include "d3d11renderer.h"
#include "util.h"
#include <iostream>

using namespace Engine::Utils;
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
	ui32 refreshDenum = 0;
	for (ui32 i = 0; i < numModes; i++)
	{
		if (modes[i].Width == width && modes[i].Height == height)
		{
			refreshNum = modes[i].RefreshRate.Numerator;
			refreshDenum = modes[i].RefreshRate.Denominator;
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
		printf("Graphics Device: %s", adapterDescText);
		printf("Graphics available Memory: %d", static_cast<ui32>(adapterDesc.DedicatedVideoMemory * 9.5367E-7f));
	}
	SAFEDELETEARR(modes);
	SAFERELEASE(output);
	SAFERELEASE(adapter);
	SAFERELEASE(factory);

	return true;
}

void Engine::Graphics::D3D11Renderer::Run()
{

}

void Engine::Graphics::D3D11Renderer::Shutdown()
{

}
