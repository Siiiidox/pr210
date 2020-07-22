#pragma once

#include "vec3.h"
#include "vec2.h"
#include "vertex.h"

namespace Engine::Resources
{
	class Mesh
	{
	private:

	public:
		void GenerateQuad();
		void GenerateCube();
		void GenerateRoundedCube();
		void GenerateSphere(int rings, int segments);
	};

}