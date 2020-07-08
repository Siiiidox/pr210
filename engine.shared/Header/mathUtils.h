#pragma once

#include "types.h"
#include <cassert>

namespace Engine::Math
{
	static constexpr real PI = static_cast<real>(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
	static constexpr real PI_2 = PI / static_cast<real>(2.0);

	static constexpr real ANGLETORAD()
	{
		return PI / static_cast<real>(180.0);
	}
	static constexpr real ANGLETODEG()
	{
		return static_cast<real>(180.0) / PI;
	}

	template<typename T>
	static constexpr T MAX(const T& val, const T& max)
	{
		return (val > max)* val + (val <= max) * max;
	}
	template<typename T>
	static constexpr T MIN(const T& val, const T& min)
	{
		return (val > min)* min + (val <= min) * val;
	}
	template<typename T>
	static constexpr T CLAMP(const T& val, const T& min, const T& max)
	{
		assert(min <= max);
		return MAX(MIN(val, max), min);
	}


}