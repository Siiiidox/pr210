#include <iostream>
#include <d3dcompiler.h>

#include "d3d11renderer.h"
#include "utils.h"
#include "vec3.h"
#include "vec2.h"
#include "color.h"
#include "mesh.h"
#include "vertex.h"
#include "matrix4x4.h"



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

	//Create Rasterizer to have a Solid Object.
	ID3D11RasterizerState* rasterState = nullptr;
	D3D11_RASTERIZER_DESC rasterDesc = { };
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	if(FAILED(device->CreateRasterizerState(&rasterDesc, &rasterState)))
	{
		MessageBoxA(NULL, "Could not create Camerabuffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	immediateContext->RSSetState(rasterState);

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


	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	bufferDesc.ByteWidth = sizeof(Engine::Math::Matrix4x4);

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferSubResource = {};
	bufferSubResource.pSysMem = &Engine::Math::Matrix4x4::IDENTITY;
	bufferSubResource.SysMemPitch = 0;
	bufferSubResource.SysMemSlicePitch = 0;
	ID3D11Buffer* buffer = nullptr;

	//Tell device to create Vertex Buffer and the data in subresource
	if (FAILED(device->CreateBuffer(&bufferDesc, &bufferSubResource, &buffer)))
	{
		MessageBoxA(NULL, "Could not create Camerabuffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	transformBuffer = reinterpret_cast<GraphicsBufferPtr>(buffer);
	if (!transformBuffer)
	{
		MessageBoxA(NULL, "Could not set camera", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}


	//TODO
	//Engine::Resources::Mesh mesh = Engine::Resources::Mesh();
	//mesh.GenerateQuad(device,quadVertexBuffer, quadIndexBuffer);
	//
	CreateShader();

	return true;
}

void Engine::Graphics::D3D11Renderer::Shutdown()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}

	SAFERELEASE(vertexLayout);
	SAFERELEASE(vertexShader);
	SAFERELEASE(pixelShader);

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


void Engine::Graphics::D3D11Renderer::RenderCube(Engine::Math::Quaternion rotation, Engine::Resources::Cube cube)
{
	//Input Assembler Stage
	ui32 stride = sizeof(Engine::Graphics::Vertex);
	ui32 offset = 0;


	Engine::Math::Matrix4x4 objectMatrix =
		Engine::Math::Matrix4x4::FromTranslation(cube.transform.position)
		* Engine::Math::Matrix4x4::FromOrientation(rotation)
		* Engine::Math::Matrix4x4::FromScale(cube.transform.scale);


	//Map the contents of the transformBuffer in CPU memory to be accessed by CPU
	D3D11_MAPPED_SUBRESOURCE modelResource = {};
	if (FAILED(immediateContext->Map(reinterpret_cast<ID3D11Resource*>(transformBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &modelResource)))
	{
		MessageBoxA(NULL, "could not map transform buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//Convert mapped data to desired type
	Engine::Math::Matrix4x4* dataMat = reinterpret_cast<Engine::Math::Matrix4x4*>(modelResource.pData);

	//Assign new data to be uploaded
	if (dataMat)(*dataMat) = objectMatrix;

	//Unmap to confirm upload and discard old data
	immediateContext->Unmap(reinterpret_cast<ID3D11Resource*>(transformBuffer), 0);

	//Bind the transformation for rendering this object
	immediateContext->VSSetConstantBuffers(0, 1, reinterpret_cast<ID3D11Buffer**>(&transformBuffer));

	immediateContext->VSSetConstantBuffers(1, 1, reinterpret_cast<ID3D11Buffer**>(&cameraBuffer));


	immediateContext->IASetVertexBuffers(0, 1, &cube.quadVertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(cube.quadIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->IASetInputLayout(vertexLayout);

	//Vertex Shader Stage
	immediateContext->VSSetShader(vertexShader, nullptr, 0);

	//Pixel Shader Stage
	immediateContext->PSSetShader(pixelShader, nullptr, 0);

	//Ouput Merger Stage
	immediateContext->OMSetRenderTargets(1, &targetView, nullptr);

	immediateContext->DrawIndexed(cube.indexCount, 0, 0);
}

void Engine::Graphics::D3D11Renderer::CreateShader()
{
	ID3DBlob* vertexShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"./data/shd/vertexDefault.shader", &vertexShaderBlob)))
	{
		printf("Caught Error: D3DReadFileToBlob(vertex) FAILED!\n");
		return;
	}

	ID3DBlob* indexShaderBlob = nullptr;
	if (FAILED(D3DReadFileToBlob(L"./data/shd/pixelDefault.shader", &indexShaderBlob)))
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

void Engine::Graphics::D3D11Renderer::SetActiveCamera(const Camera& camera)
{
	if (!activeCamera)
	{
		activeCamera = &camera;

		//Create a buffer with the calculated matrix of the camera
		Engine::Math::Matrix4x4 viewProjMat = Engine::Math::Matrix4x4::IDENTITY;
		if (activeCamera->cameraType == Camera::CameraType::Perspective)
		{
			viewProjMat = Engine::Math::Matrix4x4::FromPerspectiveFOV(activeCamera->h_fov, static_cast<real>(width) / static_cast<real>(height), activeCamera->near_plane, activeCamera->far_plane)* Engine::Math::Matrix4x4::FromView(activeCamera->transform.position);
			//viewProjMat = Engine::Math::Matrix4x4::IDENTITY * Engine::Math::Matrix4x4::FromView(activeCamera->transform.position);
		}
		else
		{
			viewProjMat = Engine::Math::Matrix4x4::FromOrthographic(static_cast<real>(4), static_cast<real>(4), activeCamera->near_plane, activeCamera->far_plane)* Engine::Math::Matrix4x4::FromView(activeCamera->transform.position);
		}

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		bufferDesc.ByteWidth = sizeof(Engine::Math::Matrix4x4);

		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferSubResource = {};
		bufferSubResource.pSysMem = &viewProjMat;
		bufferSubResource.SysMemPitch = 0;
		bufferSubResource.SysMemSlicePitch = 0;
		ID3D11Buffer* buffer = nullptr;

		//Tell device to create Vertex Buffer and the data in subresource
		if (FAILED(device->CreateBuffer(&bufferDesc, &bufferSubResource, &buffer)))
		{
			MessageBoxA(NULL, "Could not create Camerabuffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		cameraBuffer = reinterpret_cast<GraphicsBufferPtr>(buffer);
		if (!cameraBuffer)
		{
			MessageBoxA(NULL, "Could not set camera", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		}
	}
}
