#pragma once

#include <cmath>
#include <cfloat>
#include <cassert>

#include "types.h"




namespace Engine::Math
{
	class Vec3
	{
	public:
		real x = static_cast<real>(0.0);
		real y = static_cast<real>(0.0);
		real z = static_cast<real>(0.0);

		static const Vec3 ZERO;
		static const Vec3 UNITX;
		static const Vec3 UNITY;
		static const Vec3 UNITZ;
		static const Vec3 UNITSCALE;

		Vec3(real x = static_cast<real>(0.0), real y = static_cast<real>(0.0), real z = static_cast<real>(0.0)) :
			x(x),
			y(y),
			z(z)
		{
		}


		inline bool Normalize()
		{
			real invertedMagnitude = 1.0f / Magnitude();

			if (invertedMagnitude >= FLT_MIN)
			{
				*this *= invertedMagnitude;
				return true;
			}
			else
			{
				return false;
			}
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
			return (x * x) + (y * y) + (z * z);
		}

		inline real Dot(Vec3 vector) const
		{
			return (x * vector.x) + (y * vector.y) + (z * vector.z);
		}
		inline Vec3 Cross(Vec3 vector) const
		{
			return Vec3{ (y * vector.z - z * vector.y), (z * vector.x - x * vector.z), (x * vector.y - y * vector.x) };
		}
		inline Vec3 Lerp(Vec3 vector, real time) const
		{
			return (*this + (vector - *this) * time);
		}
		inline real Distance(Vec3 vector) const
		{
#ifdef DOUBLEPRECISION
			return sqrt((vector - *this).SqrMagnitude());
#else
			return sqrtf((vector - *this).SqrMagnitude());
#endif		
		}

		inline Vec3& operator+=(const Vec3& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}
		inline Vec3& operator-=(const Vec3& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}
		inline Vec3& operator*=(const Vec3& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			return *this;
		}
		inline Vec3& operator/=(const Vec3& rhs)
		{
			assert(rhs != Vec3::ZERO);

			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			return *this;
		}

		inline Vec3& operator*=(const real rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			this->z *= rhs;
			return *this;
		}
		inline Vec3& operator/=(const real rhs)
		{
			assert(rhs != static_cast<real>(0.0));

			this->x /= rhs;
			this->y /= rhs;
			this->z /= rhs;
			return *this;
		}

		inline Vec3 operator+(const Vec3& vector) const
		{
			return Vec3{ x + vector.x, y + vector.y, z + vector.z };
		}
		inline Vec3 operator-(const Vec3& vector) const
		{
			return Vec3{ x - vector.x, y - vector.y, z - vector.z };
		}
		inline Vec3 operator*(const Vec3& vector) const
		{
			return Vec3{ x * vector.x, y * vector.y, z * vector.z };
		}
		inline Vec3 operator/(const Vec3& vector) const
		{
			assert(vector != Vec3::ZERO);

			return Vec3{ x / vector.x, y / vector.y, z / vector.z };
		}

		inline Vec3 operator*(const real t) const
		{
			return Vec3{ x * t, y * t, z * t };
		}
		inline Vec3 operator/(const real t) const
		{
			assert(t != static_cast<real>(0.0));

			return Vec3{ x / t, y / t, z / t };
		}

		inline bool operator==(const Vec3& vector) const
		{
			if (x == vector.x && y == vector.y && z == vector.z)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool operator!=(const Vec3& vector) const
		{
			return !(*this == vector);
		}

		inline Vec3 operator-() const
		{
			return Vec3{ -x,-y,-z };
		}

		//Matrix
		Vec3& operator*=(const class Matrix4x4& matrix);
	};


}