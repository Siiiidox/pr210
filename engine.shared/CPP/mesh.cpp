#include "mesh.h"
#include "mathUtils.h"
#include "utils.h"
#include <iostream>

struct vertexType
{
	Engine::Math::Vec3 position;
	Engine::Utils::FloatColor color;
	Engine::Math::Vec3 normal;
	Engine::Math::Vec2 textCoords;
};

/*Engine::Resources::Cube::~Cube()
{
	SAFERELEASE(quadVertexBuffer);
	SAFERELEASE(quadIndexBuffer);
}*/

Engine::Resources::Cube Engine::Resources::Mesh::GenerateCube(Engine::Math::Transform transform, ID3D11Device* device)
{
	Engine::Resources::Cube cube = { };
	cube.transform = transform;
	Engine::Graphics::Vertex vertices[] =
	{
		{ Engine::Math::Vec3{-0.5f,  0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 0.f, 1.f, 1.f}, Engine::Math::Vec3{-0.5f,  0.5f, -0.5f}, Engine::Math::Vec2{1.f,1.f} },
		{ Engine::Math::Vec3{ 0.5f,  0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 0.f, 0.f, 1.f}, Engine::Math::Vec3{ 0.5f,  0.5f, -0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{-0.5f, -0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 0.f, 0.f, 1.f}, Engine::Math::Vec3{-0.5f, -0.5f, -0.5f}, Engine::Math::Vec2{0.f,1.f} },
		{ Engine::Math::Vec3{ 0.5f, -0.5f, -0.5f}, Engine::Utils::FloatColor{1.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{ 0.5f, -0.5f, -0.5f}, Engine::Math::Vec2{1.f,0.f} },

		{ Engine::Math::Vec3{-0.5f,  0.5f,  0.5f}, Engine::Utils::FloatColor{1.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{-0.5f,  0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{ 0.5f,  0.5f,  0.5f}, Engine::Utils::FloatColor{0.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{ 0.5f,  0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{-0.5f, -0.5f,  0.5f}, Engine::Utils::FloatColor{0.f, 1.f, 0.f, 1.f}, Engine::Math::Vec3{-0.5f, -0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} },
		{ Engine::Math::Vec3{ 0.5f, -0.5f,  0.5f}, Engine::Utils::FloatColor{0.f, 1.f, 1.f, 1.f}, Engine::Math::Vec3{ 0.5f, -0.5f,  0.5f}, Engine::Math::Vec2{1.f,0.f} }
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

	cube.vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	cube.vertexBufferDesc.ByteWidth = sizeof(vertices);
	cube.vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	cube.vertexBufferDesc.CPUAccessFlags = 0;
	cube.vertexBufferDesc.MiscFlags = 0;
	cube.vertexBufferDesc.StructureByteStride = 0;

	cube.vertexResourceData.pSysMem = &vertices;
	cube.vertexResourceData.SysMemPitch = 0;
	cube.vertexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&cube.vertexBufferDesc, &cube.vertexResourceData, &cube.quadVertexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(vertex) FAILED!\n");
		return { };
	}

	cube.indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	cube.indexBufferDesc.ByteWidth = sizeof(indices);
	cube.indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	cube.indexBufferDesc.CPUAccessFlags = 0;
	cube.indexBufferDesc.MiscFlags = 0;
	cube.indexBufferDesc.StructureByteStride = 0;

	cube.indexResourceData.pSysMem = &indices;
	cube.indexResourceData.SysMemPitch = 0;
	cube.indexResourceData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&cube.indexBufferDesc, &cube.indexResourceData, &cube.quadIndexBuffer)))
	{
		printf("Caught Error: device->CreateBuffer(index) FAILED!\n");
		return { };
	}
	return cube;
}

void Engine::Resources::Mesh::GenerateQuad(ID3D11Device* device, ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer)
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
