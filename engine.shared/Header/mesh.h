#pragma once

#include "vec3.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include "..\..\engine.client\Header\d3d11renderer.h"
#include "vertex.h"
#include "transform.h"

namespace Engine::Resources
{
	/*!
	*	@brief MeshObject which stores all required Data for the MeshObject
	*/
	struct MeshObject
	{
		/*!
		*	@brief variable to store the transform 
		*/
		Engine::Math::Transform transform;
		/*!
		*	@brief variable to store the vertices
		*/
		Engine::Graphics::Vertex* vertices = nullptr;
		/*!
		*	@brief variable to store the indexCount
		*/
		int indexCount = 36;
		/*!
		*	@brief variable to store the indicies
		*/
		int* indices = nullptr;

		/*!
		*	@brief variable to store the buffer for the vertices
		*/
		ID3D11Buffer* vertexBuffer = nullptr;
		/*!
		*	@brief variable to store the buffer for the indices
		*/
		ID3D11Buffer* indexBuffer = nullptr;

		/*!
		*	@brief varialbe to store the sampler for the texture
		*/
		ID3D11SamplerState* sampler = nullptr;
		/*!
		*	@brief varialbe to store the ShaderRessourceView
		*/
		ID3D11ShaderResourceView* resourceView = nullptr;
	};

	class Mesh
	{
	public:
		/*!
		*	@brief Generate a Quad
		*	@param Transform
		*	@param ID3D11Device
		*	@return MeshObject
		*/
		static Engine::Resources::MeshObject GenerateQuad(Engine::Math::Transform transform, ID3D11Device* device);
		
		/*!
		*	@brief Generate a Cube
		*	@param Transform
		*	@param ID3D11Device
		*	@return MeshObject
		*/
		static Engine::Resources::MeshObject GenerateCube(Engine::Math::Transform transform, ID3D11Device* device);



	};

}