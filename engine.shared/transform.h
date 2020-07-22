#pragma once
#include "vec3.h"
#include "quaternion.h"
namespace Engine::Math
{
	struct Transform
	{
		Engine::Math::Vec3 position = Engine::Math::Vec3::Zero;
		Engine::Math::Quaternion rotation = Engine::Math::Quaternion::Zero;
		Engine::Math::Vec3 scale = Engine::Math::Vec3::UnitScale;
	};
}