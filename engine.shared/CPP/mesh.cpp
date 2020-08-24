#include "mesh.h"
#include "mathUtils.h"
#include "utils.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Engine::Resources::MeshObject Engine::Resources::Mesh::GenerateCube(Engine::Math::Transform transform, ID3D11Device* device)
{
	Engine::Resources::MeshObject cube = { };
	cube.transform = transform;
	Engine::Graphics::Vertex vertices[] =
	{
		{ Engine::Math::Vec3{-0.5f,  0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 0.f, 0.f, 1.f}, Engine::Math::Vec3{-0.5f,  0.5f, -0.5f}, Engine::Math::Vec2{1.f,1.f} },
		{ Engine::Math::Vec3{ 0.5f,  0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{ 0.5f,  0.5f, -0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{-0.5f, -0.5f, -0.5f}, Engine::Utils::FloatColor{0.f, 1.f, 1.f, 1.f}, Engine::Math::Vec3{-0.5f, -0.5f, -0.5f}, Engine::Math::Vec2{0.f,1.f} },
		{ Engine::Math::Vec3{ 0.5f, -0.5f, -0.5f}, Engine::Utils::FloatColor{0.f, 0.f, 1.f, 1.f}, Engine::Math::Vec3{ 0.5f, -0.5f, -0.5f}, Engine::Math::Vec2{1.f,0.f} },

		{ Engine::Math::Vec3{-0.5f,  0.5f,  0.5f}, Engine::Utils::FloatColor{0.f, 0.f, 1.f, 1.f}, Engine::Math::Vec3{-0.5f,  0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{ 0.5f,  0.5f,  0.5f}, Engine::Utils::FloatColor{0.f, 1.f, 1.f, 1.f}, Engine::Math::Vec3{ 0.5f,  0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{-0.5f, -0.5f,  0.5f}, Engine::Utils::FloatColor{1.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{-0.5f, -0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{ 0.5f, -0.5f,  0.5f}, Engine::Utils::FloatColor{1.f, 0.f, 0.f, 1.f}, Engine::Math::Vec3{ 0.5f, -0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} }
	};
	cube.vertices = vertices;

	//index the verices counterclock wise
	//Counterclock wise because D3D uses it by default
	int indices[] =
	{
		0,1,2,2,1,3,
		4,0,6,6,0,2,
		1,5,3,3,5,7,
		5,4,7,4,6,7,
		5,0,4,1,0,5,
		2,3,7,6,2,7
	};
	cube.indexCount = sizeof(indices)/sizeof(int);
	cube.indices = indices;

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

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexResourceData, &cube.vertexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(vertex) FAILED!\n");
		return { };
	}

	D3D11_BUFFER_DESC indexBufferDesc = { };
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexResourceData = { };
	indexResourceData.pSysMem = &indices;
	indexResourceData.SysMemPitch = 0;
	indexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexResourceData, &cube.indexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(index) FAILED!\n");
		return { };
	}
	return cube;
}

Engine::Resources::MeshObject Engine::Resources::Mesh::GenerateQuad(Engine::Math::Transform transform, ID3D11Device* device)
{
	Engine::Resources::MeshObject quad = { };
	quad.transform = transform;

	Engine::Graphics::Vertex vertices[4] =
	{
		{ Engine::Math::Vec3 {-0.5f, -0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 0.f, 0.f} },
		{ Engine::Math::Vec3 { 0.5f, -0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 1.f, 0.f} },
		{ Engine::Math::Vec3 { -0.5f, 0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 0.f, 1.f} },
		{ Engine::Math::Vec3 { 0.5f, 0.5f, 0.f }, Engine::Utils::FloatColor { .5f,.5f,.5f, 1.f }, Engine::Math::Vec3 { 0.f, 0.f, 1.f }, Engine::Math::Vec2{ 1.f, 1.f} }
	};

	quad.vertices = vertices;

	int indices[6] = { 0, 1, 2, 3, 2, 1 };

	quad.indexCount = sizeof(indices) / sizeof(int);
	quad.indices = indices;

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

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexResourceData, &quad.vertexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(vertex) FAILED!\n");
		return { };
	}

	D3D11_BUFFER_DESC indexBufferDesc = { };
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexResourceData = { };
	indexResourceData.pSysMem = &indices;
	indexResourceData.SysMemPitch = 0;
	indexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexResourceData, &quad.indexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(index) FAILED!\n");
		return { };
	}

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./data/font_test.png", &width , &height, &channels, STBI_default);
	if(data == nullptr)
	{
		printf("Caught Error: not able to load texture!\n");
		return {};
	}
	D3D11_TEXTURE2D_DESC textureBufferDesc = { };
	textureBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	textureBufferDesc.Width = width;
	textureBufferDesc.Height = height;
	textureBufferDesc.MipLevels = 1;
	textureBufferDesc.ArraySize = 1;
	textureBufferDesc.SampleDesc.Count = 1;
	textureBufferDesc.SampleDesc.Quality = 0;
	textureBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	textureBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureBufferDesc.MiscFlags = 0;
	textureBufferDesc.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA textureResourceData = { };
	textureResourceData.pSysMem = data;
	textureResourceData.SysMemPitch = width * sizeof(unsigned char) * 4U;
	textureResourceData.SysMemSlicePitch = width * sizeof(unsigned char) * 4U * height;

	ID3D11Texture2D* texture2D = nullptr;

	if (FAILED(device->CreateTexture2D(&textureBufferDesc, &textureResourceData, &texture2D)))
	{
		printf("Caught Error: device->CreateBuffer(texture) FAILED!\n");
		return { };
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc = { };
	shaderDesc.Format = textureBufferDesc.Format;
	shaderDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderDesc.Texture2D.MostDetailedMip = 0;
	shaderDesc.Texture2D.MipLevels = 1;


	if (FAILED(device->CreateShaderResourceView(texture2D, &shaderDesc, &quad.resourceView)))
	{
		printf("Caught Error: device->CreateBuffer(sampler) FAILED!\n");
		return { };
	}

	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (FAILED(device->CreateSamplerState(&samplerDesc, &quad.sampler)))
	{
		printf("Caught Error: device->CreateBuffer(sampler) FAILED!\n");
		return { };
	}

	return quad;
}
