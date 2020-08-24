#pragma once

#include "vec3.h"
#include "vec2.h"
#include "color.h"

namespace Engine::Graphics
{
	/*!
	*	@brief stuct vertex which stores:
	*	position,
	*	color,
	*	normal,
	*	texCoords
	*/
	struct Vertex
	{
		Engine::Math::Vec3 position;
		Engine::Utils::FloatColor color;
		Engine::Math::Vec3 normal;
		Engine::Math::Vec2 texCoords;
	};
}