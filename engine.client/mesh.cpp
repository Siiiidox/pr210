#include "mesh.h"
#include "vertex.h"
#include "mathutils.h"
#include <iostream>



using namespace Engine::Math;
using namespace Engine::Utils;
using namespace Engine::Graphics;

void Engine::Resources::Mesh::GenerateQuad()
{
	indexCount = 6;
	//Create the verices required to represent a quad
	Vertex vertices[4] =
	{
		{ Vec3{-0.5f, -0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,1.f} },
		{ Vec3{0.5f, -0.5f, 0.0f}, FloatColor{0.f, 1.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{-0.5f, 0.5f, 0.0f}, FloatColor{0.f, 0.f, 1.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{0.f,1.f}},
		{ Vec3{0.5f, 0.5f, 0.0f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} }
	};
	//index the verices counterclock wise
	//Counterclock wise because D3D uses it by default
	int indices[6] =
	{
		0,2,1,
		1,2,3
	};
	vertexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Vertex, &vertices, sizeof(vertices));
	if (!vertexBuffer)
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	indexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Index, &indices, sizeof(indices));
	if (!indexBuffer)
	{
		MessageBoxA(NULL, "Could not create index buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
}

void Engine::Resources::Mesh::GenerateCube()
{
	indexCount = 36;
	Vertex vertices[8] =
	{
		{ Vec3{-0.5f,  0.5f, -0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,1.f} },
		{ Vec3{ 0.5f,  0.5f, -0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{-0.5f, -0.5f, -0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{0.f,1.f} },
		{ Vec3{ 0.5f, -0.5f, -0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },

		{ Vec3{-0.5f,  0.5f,  0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{ 0.5f,  0.5f,  0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{-0.5f, -0.5f,  0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} },
		{ Vec3{ 0.5f, -0.5f,  0.5f}, FloatColor{1.f, 0.f, 0.f, 1.f}, Vec3{0.f, 0.f, -1.f}, Vec2{1.f,0.f} }
	};
	//index the verices counterclock wise
	//Counterclock wise because D3D uses it by default
	int indices[36] =
	{
		0,1,2,2,1,3,
		4,0,6,6,0,2,
		1,5,3,3,5,7,
		5,4,7,4,6,7,
		5,0,4,1,0,5,
		2,3,7,6,2,7
	};
	vertexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Vertex, &vertices, sizeof(vertices));
	if (!vertexBuffer)
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	indexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Index, &indices, sizeof(indices));
	if (!indexBuffer)
	{
		MessageBoxA(NULL, "Could not create index buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
}

void Engine::Resources::Mesh::GenerateRoundedCube(float cornerRadius, int edgeCount)
{
	int vertexCount = (4 + edgeCount * 4);
	int indexCount = (6 + edgeCount * 6);


}

void Engine::Resources::Mesh::GenerateSphere(int rings, int segments)
{
	rings = Max(rings, 3);
	segments = Max(segments, 3);

	float lengthInv = 1.0f; // / radius
	float sectorStep = 2 * PI / segments;
	float stackStep = PI / rings;
	float sectorAngle, stackAngle;

	int vertexCount = (rings - 1) * segments + 2;
	int indexCount = (vertexCount - 2) * 6;
	this->indexCount = indexCount;
	Vertex* vertices = new Vertex[vertexCount];
	if (vertices == nullptr)
	{
		MessageBoxA(NULL, "Could not allocate vertices", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	int* indices = new int[indexCount];
	if (indices == nullptr)
	{
		MessageBoxA(NULL, "Could not allocate indices", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	int v = 0;
	for (int i = 0; i <= rings; ++i)
	{
		stackAngle = PI_2 - i * stackStep;
		float xy = 1.0f * cosf(stackAngle);
		float z = 1.0f * sinf(stackAngle);

		for (int j = 0; j < segments; ++j)
		{
			sectorAngle = j * sectorStep;

			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);

			vertices[v++].position = Vec3(x, y, z);

			float nx = x * lengthInv;
			float ny = y * lengthInv;
			float nz = z * lengthInv;

			vertices[v].normal = Vec3(nx, ny, nz);
			if (i == 0 || i == rings)
				break;
		}
	}
	int f = 0;
	ui32 k1, k2;
	for (int i = 0; i < rings; ++i)
	{
		k1 = (i - 1) * segments + 1;
		k2 = k1 + segments;

		for (int j = 0; j <= segments; ++j, ++k1, ++k2)
		{
			if (i == 0)
			{
				if (j != 0)
				{
					indices[f++] = 0;
					indices[f++] = j;
					indices[f++] = (j % segments) + 1;
				}
			}
			else if (i == (rings - 1))
			{
				if (j == segments)break;
				int offset = vertexCount - segments;
				indices[f++] = vertexCount - 1;
				indices[f++] = offset + ((j + 1) % segments) - 1;
				indices[f++] = offset + j - 1;
			}
			else
			{
				if (j != 0)
				{
					indices[f++] = k1 - 1;
					indices[f++] = k2 - 1;
					indices[f++] = k1;
				}
				if (j < segments)
				{
					indices[f++] = k1;
					indices[f++] = k2 - 1;
					indices[f++] = k2;
				}
			}
		}
	}
	vertexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Vertex, vertices, sizeof(Vertex) * vertexCount);
	if (!vertexBuffer)
	{
		MessageBoxA(NULL, "Could not create vertex buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	indexBuffer = D3D11Renderer::GetInstance().CreateBuffer(D3D11Renderer::BufferType::Index, indices, sizeof(int) * indexCount);
	if (!indexBuffer)
	{
		MessageBoxA(NULL, "Could not create index buffer", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
}
