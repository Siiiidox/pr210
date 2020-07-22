#pragma once

// EXTERNAL INCLUDES
#include <d3dcompiler.h>
#include <d3d11.h>
#include "d3d11renderer.h"

namespace Engine::Resources
{
	class Mesh
	{
	public:
		GraphicsBufferPtr vertexBuffer = nullptr;
		GraphicsBufferPtr indexBuffer = nullptr;
		int indexCount = 0;
	
		void GenerateQuad();
		void GenerateCube();
		void GenerateRoundedCube(float cornerRadius, int edgeCount);
		void GenerateSphere(int rings, int segments);
	};
}