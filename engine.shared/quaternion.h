#pragma once
// EXTERNAL INCLUDES
#include <cmath>
#include <cfloat>
#include <cassert>
// INTERNAL INCLUDES
#include "types.h"
#include "vec3.h"

namespace Engine::Math
{
	class Quaternion
	{
	public:
		real x = static_cast<real>(0.0);
		real y = static_cast<real>(0.0);
		real z = static_cast<real>(0.0);
		real w = static_cast<real>(0.0);

		static const Quaternion Zero;

		inline Quaternion(const Quaternion& q)
			: Quaternion(q.x, q.y, q.z, q.w)
		{ }
		inline Quaternion(real x = static_cast<real>(1.0)
			, real y = static_cast<real>(1.0)
			, real z = static_cast<real>(1.0)
			, real w = static_cast<real>(1.0)) :
			x(x),
			y(y),
			z(z),
			w(w)
		{ }

		inline bool Normalize()
		{
			real invMag = static_cast<real>(1.0) / Magnitude();

#ifdef DOUBLEPRECISION 
			if (invMag >= DBL_MIN)
#else
			if (invMag >= FLT_MIN)
#endif // DOUBLEPRECISION 
			{
				*this *= invMag;
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
			return x * x + y * y + z * z + w * w;
		}

		inline real Dot(Quaternion q) const
		{
			return	(x * q.x) +
				(y * q.y) +
				(z * q.z) +
				(w * q.w);
		}
		inline Quaternion Lerp(Quaternion v, real t) const
		{
			return (*this) + (v - (*this)) * t;
		}
		inline real Distance(Quaternion v) const
		{
#ifdef DOUBLEPRECISION
			return sqrt(SqrDistance(v));
#else
			return sqrtf(SqrDistance(v));
#endif
		}
		inline real SqrDistance(Quaternion v) const
		{
			return (v - *this).SqrMagnitude();
		}

		inline Quaternion& operator+=(const Quaternion& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;
			return *this;
		}
		inline Quaternion& operator-=(const Quaternion& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;
			return *this;
		}
		inline Quaternion& operator*=(const Quaternion& rhs)
		{
			Quaternion temp = *this;
			this->w = temp.w * rhs.w - temp.x * rhs.x - temp.y * rhs.y - temp.z * rhs.z;
			this->x = temp.w * rhs.x + temp.x * rhs.w + temp.y * rhs.z - temp.z * rhs.y;
			this->y = temp.w * rhs.y + temp.y * rhs.w + temp.z * rhs.x - temp.x * rhs.z;
			this->z = temp.w * rhs.z + temp.z * rhs.w + temp.x * rhs.y - temp.y * rhs.x;
			return *this;
		}
		inline Quaternion& operator/=(const Quaternion& rhs)
		{
			assert(rhs != Quaternion::Zero);
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			this->w /= rhs.w;
			return *this;
		}
		inline Quaternion& operator*=(const real rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			this->z *= rhs;
			this->w *= rhs;
			return *this;
		}
		//Quaternion& operator*=(const class Mat4x4& rhs);
		inline Quaternion& operator/=(const real rhs)
		{
			assert(rhs != static_cast<real>(0.0));
			this->x /= rhs;
			this->y /= rhs;
			this->z /= rhs;
			this->w /= rhs;
			return *this;
		}

		inline Quaternion operator+(const Quaternion& q) const
		{
			return Quaternion{ x + q.x, y + q.y, z + q.z, w + q.w};
		}
		inline Quaternion operator-(const Quaternion& q) const
		{
			return Quaternion{ x - q.x, y - q.y, z - q.z, w - q.w};
		}
		inline Quaternion operator*(const Quaternion& q) const
		{
			return Quaternion{ x * q.x, y * q.y, z * q.z, w * q.w };
		}
		inline Quaternion operator/(const Quaternion& q) const
		{
			assert(q != Quaternion::Zero);
			return Quaternion{ x / q.x, y / q.y, z / q.z, w / q.w };
		}
		inline Quaternion operator*(const real t) const
		{
			return Quaternion{ x * t, y * t, z * t, w * t };
		}
		inline Quaternion operator/(const real t) const
		{
			assert(t != static_cast<real>(0.0));
			return Quaternion{ x / t, y / t, z / t, w / t };
		}

		inline bool operator==(const Quaternion& q) const
		{
			if (x == q.x && y == q.y && z == q.z && w == q.w)
				return true;
			return false;
		}
		inline bool operator!=(const Quaternion& v) const
		{
			return !(*this == v);
		}

		inline Quaternion operator-() const
		{
			return Quaternion{ -x, -y, -z, -w };
		}

		static Quaternion FromAngleAxis(real angle, const Vec3& axis)
		{
			real halfAngle = angle * static_cast<real>(0.5);
#ifdef DOUBLEPRECISION 
			real halfSin = sin(halfAngle);
			real halfCos = cos(halfAngle);
#else
			real halfSin = sinf(halfAngle);
			real halfCos = cosf(halfAngle);
#endif
			return Quaternion
			{
				halfSin * axis.x,
				halfSin * axis.y,
				halfSin * axis.z,
				halfCos
			};
		}
	};
}