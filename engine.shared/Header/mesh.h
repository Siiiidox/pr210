#pragma once

#include "vec3.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include "..\..\engine.client\Header\d3d11renderer.h"
#include "vertex.h"
#include "transform.h"

namespace Engine::Resources
{
	struct Cube
	{
		Engine::Math::Transform transform;
		Engine::Graphics::Vertex* vertices = nullptr;
		int indexCount = 36;
		int* indices = nullptr;
		D3D11_BUFFER_DESC vertexBufferDesc = { };
		D3D11_SUBRESOURCE_DATA vertexResourceData = { };
		D3D11_BUFFER_DESC indexBufferDesc = { };
		D3D11_SUBRESOURCE_DATA indexResourceData = { };

		ID3D11Buffer* quadVertexBuffer = nullptr;
		ID3D11Buffer* quadIndexBuffer = nullptr;

		//~Cube();
	};
	class Mesh
	{
	private:

	public:
		void GenerateQuad(ID3D11Device* device, ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer);
		static Engine::Resources::Cube GenerateCube(Engine::Math::Transform transform, ID3D11Device* device);



	};

}