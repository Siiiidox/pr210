#pragma once


#include <cmath>
#include <cfloat>
#include <cassert>

#include "types.h"

namespace Engine::Math
{
	class Vec2
	{
	public:

		real x = static_cast<real>(0.0);
		real y = static_cast<real>(0.0);

		static const Vec2 ZERO;
		static const Vec2 UNITX;
		static const Vec2 UNITY;
		static const Vec2 UNITSCALE;

		inline Vec2(const Vec2& vec)
			: Vec2(vec.x, vec.y)
		{
		}
		inline Vec2(real x = static_cast<real>(0.0)
			, real y = static_cast<real>(0.0)) :
			x(x),
			y(y)
		{
		}

		inline bool Normalize()
		{
			real invertedMagnitude = static_cast<real>(1.0) / Magnitude();

#ifdef DOUBLEPRECISION 
			if (invertedMagnitude >= DBL_MIN)
#else
			if (invertedMagnitude >= FLT_MIN)
#endif // DOUBLEPRECISION 
			{
				*this *= invertedMagnitude;
				return true;
			}

			return false;
		}
		inline real Magnitude() const
		{
#ifdef DOUBLEPRECISION
			return sqrt(SqrMagnitude());
#else
			return sqrtf(SqrMagnitude());
#endif
		}
		inline real SqrMagnitude() const
		{
			return x * x + y * y;
		}

		inline real Dot(Vec2 vector) const
		{
			return	(x * vector.x) +
				(y * vector.y);
		}
		inline real Cross(Vec2 vector) const
		{
			return  x * vector.y - y * vector.x;
		}
		inline Vec2 Lerp(Vec2 vector, real time) const
		{
			return (*this) + (vector - (*this)) * time;
		}
		inline real Distance(Vec2 vector) const
		{
#ifdef DOUBLEPRECISION
			return sqrt(SqrDistance(vector));
#else
			return sqrtf(SqrDistance(vector));
#endif
		}
		inline real SqrDistance(Vec2 vector) const
		{
			return (vector - *this).SqrMagnitude();
		}

		inline Vec2& operator+=(const Vec2& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}
		inline Vec2& operator-=(const Vec2& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}
		inline Vec2& operator*=(const Vec2& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			return *this;
		}
		inline Vec2& operator/=(const Vec2& rhs)
		{
			assert(rhs != Vec2::ZERO);
			this->x /= rhs.x;
			this->y /= rhs.y;
			return *this;
		}
		inline Vec2& operator*=(const real rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			return *this;
		}
		inline Vec2& operator/=(const real rhs)
		{
			assert(rhs != static_cast<real>(0.0));
			this->x /= rhs;
			this->y /= rhs;
			return *this;
		}

		inline Vec2 operator+(const Vec2& vector) const
		{
			return Vec2{ x + vector.x, y + vector.y };
		}
		inline Vec2 operator-(const Vec2& vector) const
		{
			return Vec2{ x - vector.x, y - vector.y };
		}
		inline Vec2 operator*(const Vec2& vector) const
		{
			return Vec2{ x * vector.x, y * vector.y };
		}
		inline Vec2 operator/(const Vec2& vector) const
		{
			assert(vector != Vec2::ZERO);
			return Vec2{ x / vector.x, y / vector.y };
		}
		inline Vec2 operator*(const real t) const
		{
			return Vec2{ x * t, y * t };
		}
		inline Vec2 operator/(const real t) const
		{
			assert(t != static_cast<real>(0.0));
			return Vec2{ x / t, y / t };
		}

		inline bool operator==(const Vec2& vector) const
		{
			if (x == vector.x && y == vector.y)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		inline bool operator!=(const Vec2& vector) const
		{
			return !(*this == vector);
		}

		inline Vec2 operator-()
		{
			return Vec2{ -x, -y };
		}
	};
}