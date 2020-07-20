#pragma once


#include <cmath>
#include <cfloat>
#include <cassert>

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

		static const Quaternion ZERO;

		inline Quaternion(const Quaternion& quat)
			: Quaternion(quat.x, quat.y, quat.z, quat.w)
		{
		}
		inline Quaternion(real x = static_cast<real>(0.0), real y = static_cast<real>(0.0), real z = static_cast<real>(0.0), real w = static_cast<real>(0.0)) :
			x(x),
			y(y),
			z(z),
			w(w)
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
			return x * x + y * y + z * z + w * w;
		}

		inline real Dot(Quaternion quat) const
		{
			return	(x * quat.x) +
				(y * quat.y) +
				(z * quat.z) +
				(w * quat.w);
		}
		inline Quaternion Lerp(Quaternion quat, real time) const
		{
			return (*this) + (quat - (*this)) * time;
		}
		inline real Distance(Quaternion quat) const
		{
#ifdef DOUBLEPRECISION
			return sqrt(SqrDistance(quat));
#else
			return sqrtf(SqrDistance(quat));
#endif
		}
		inline real SqrDistance(Quaternion quat) const
		{
			return (quat - *this).SqrMagnitude();
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
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			this->w *= rhs.w;
			return *this;
		}
		inline Quaternion& operator/=(const Quaternion& rhs)
		{
			assert(rhs != Quaternion::ZERO);
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
		inline Quaternion& operator/=(const real rhs)
		{
			assert(rhs != static_cast<real>(0.0));
			this->x /= rhs;
			this->y /= rhs;
			this->z /= rhs;
			this->w /= rhs;
			return *this;
		}

		inline Quaternion operator+(const Quaternion& quat) const
		{
			return Quaternion{ x + quat.x, y + quat.y, z + quat.z, w + quat.w };
		}
		inline Quaternion operator-(const Quaternion& quat) const
		{
			return Quaternion{ x - quat.x, y - quat.y, z - quat.z, w - quat.w };
		}
		inline Quaternion operator*(const Quaternion& quat) const
		{
			return Quaternion{ x * quat.x, y * quat.y,z * quat.z, w * quat.w };
		}
		inline Quaternion operator/(const Quaternion& quat) const
		{
			assert(quat != Quaternion::ZERO);
			return Quaternion{ x / quat.x, y / quat.y, z / quat.z, w / quat.w };
		}
		inline Quaternion operator*(const real t) const
		{
			return Quaternion{ x * t, y * t, z * t, w * t };
		}
		inline Quaternion operator/(const real t) const
		{
			assert(t != static_cast<real>(0.0));
			return Quaternion{ x / t, y / t, z / t,w / t };
		}

		inline bool operator==(const Quaternion& quat) const
		{
			if (this->x == quat.x && this->y == quat.y && this->z == quat.z && this->w == quat.w)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		inline bool operator!=(const Quaternion& quat) const
		{
			return !(*this == quat);
		}

		inline Quaternion operator-()
		{
			return Quaternion{ -x, -y , -z, -w };
		}
	};
}