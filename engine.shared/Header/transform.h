#pragma once
#include "vec3.h"
#include "quaternion.h"

namespace Engine::Math
{
	/*!
	*	@brief struct which stores variables as a Transform:
	*	position,
	*	rotation,
	*	scale
	*/
	struct Transform
	{		
		Engine::Math::Vec3 position = Engine::Math::Vec3::ZERO;
		Engine::Math::Quaternion rotation = Engine::Math::Quaternion::ZERO;
		Engine::Math::Vec3 scale = Engine::Math::Vec3::UNITSCALE;
	};
}