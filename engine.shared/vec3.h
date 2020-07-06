#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "types.h"

namespace Engine::Math
{
	class Vec3
	{
	public:
		
		real x = static_cast<real>(0.0);
		real y = static_cast<real>(0.0);
		real z = static_cast<real>(0.0);
		
		static const Vec3 Zero;
		static const Vec3 UnitX;
		static const Vec3 UnitY;
		static const Vec3 UnitZ;
		static const Vec3 UnitScale;
		
	};
}
