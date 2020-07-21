#include <iostream>
#include <d3dcompiler.h>

#include "d3d11renderer.h"
#include "utils.h"
#include "vec3.h"
#include "vec2.h"
#include "color.h"


struct vertexType
{
	Engine::Math::Vec3 position;
	Engine::Utils::FloatColor color;
	Engine::Math::Vec3 normal;
	Engine::Math::Vec2 textCoords;
};

void Engine::Graphics::D3D11Renderer::GenerateQuad()
{
	vertexType vertices[4] =
	{
		{ Engine::Math::Vec3 {-0.5f, -0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 0.f, 0.f} },
		{ Engine::Math::Vec3 { 0.5f, -0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 1.f, 0.f} },
		{ Engine::Math::Vec3 { -0.5f, 0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 0.f, 1.f} },
		{ Engine::Math::Vec3 { 0.5f, 0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 1.f, 1.f} }
	};

	int index[6] = { 0, 2, 1, 1, 2, 3 };


	D3D11_BUFFER_DESC vertexBufferDesc = { };
	vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexResourceData = { };
	vertexResourceData.pSysMem = &vertices;
	vertexResourceData.SysMemPitch = 0;
	vertexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexResourceData, &quadVertexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(vertex) FAILED!\n");
		return;
	}

	D3D11_BUFFER_DESC indexBufferDesc = { };
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(index);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexResourceData = { };
	indexResourceData.pSysMem = &index;
	indexResourceData.SysMemPitch = 0;
	indexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexResourceData, &quadIndexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(index) FAILED!\n");
		return;
	}
}

void Engine::Graphics::D3D11Renderer::RenderQuad()
{
	//Input Assembler Stage
	ui32 stride = sizeof(vertexType);
	ui32 offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &quadVertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->IASetInputLayout(vertexLayout);

	//Vertex Shader Stage
	immediateContext->VSSetShader(vertexShader, nullptr, 0);

	//Pixel Shader Stage
	immediateContext->PSSetShader(pixelShader, nullptr, 0);

	//Ouput Merger Stage
	immediateContext->OMSetRenderTargets(1, &targetView, nullptr);

	immediateContext->DrawIndexed(6, 0, 0);
}

void Engine::Graphics::D3D11Renderer::CreateShader()
{
	ID3DBlob* vertexShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"../bin/data/shd/vertexDefault.shader", &vertexShaderBlob)))
	{
		printf("Caught Error: D3DReadFileToBlob(vertex) FAILED!\n");
		return;
	}

	ID3DBlob* indexShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"../bin/data/shd/pixelDefault.shader", &indexShaderBlob)))
	{
		printf("Caught Error: D3DReadFileToBlob(index) FAILED!\n");
		return;
	}


	if (FAILED(device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)))
	{
		printf("Caught Error: CreateVertexShader() FAILED!\n");
		return;
	}

	if (FAILED(device->CreatePixelShader(indexShaderBlob->GetBufferPointer(), indexShaderBlob->GetBufferSize(), nullptr, &pixelShader)))
	{
		printf("Caught Error: CreateVertexShader() FAILED!\n");
		return;
	}

	D3D11_INPUT_ELEMENT_DESC vertexElementDesc[4] = { };
	vertexElementDesc[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	vertexElementDesc[0].InputSlot = 0;
	vertexElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElementDesc[0].AlignedByteOffset = 0;
	vertexElementDesc[0].SemanticIndex = 0;
	vertexElementDesc[0].SemanticName = "POSITION";

	vertexElementDesc[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertexElementDesc[1].InputSlot = 0;
	vertexElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElementDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElementDesc[1].SemanticIndex = 0;
	vertexElementDesc[1].SemanticName = "COLOR";

	vertexElementDesc[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	vertexElementDesc[2].InputSlot = 0;
	vertexElementDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElementDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElementDesc[2].SemanticIndex = 0;
	vertexElementDesc[2].SemanticName = "NORMAL";

	vertexElementDesc[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	vertexElementDesc[3].InputSlot = 0;
	vertexElementDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vertexElementDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vertexElementDesc[3].SemanticIndex = 0;
	vertexElementDesc[3].SemanticName = "TEXCOORD";

	if (FAILED(device->CreateInputLayout(vertexElementDesc, sizeof(vertexElementDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &vertexLayout)))
	{
		printf("Caught Error: device->CreateInputLayout FAILED!\n");
		return;
	}
}

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


	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(0
		, D3D_DRIVER_TYPE_HARDWARE
		, 0
		, D3D11_CREATE_DEVICE_DEBUG
		, &featureLevel
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

	GenerateQuad();
	CreateShader();

	return true;
}

void Engine::Graphics::D3D11Renderer::Shutdown()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}

	SAFERELEASE(quadVertexBuffer);

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
