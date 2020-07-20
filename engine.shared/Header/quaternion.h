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

		inline Quaternion& operator+=(const Quaternion& quat)
		{
			this->x += quat.x;
			this->y += quat.y;
			this->z += quat.z;
			this->w += quat.w;
			return *this;
		}
		inline Quaternion& operator-=(const Quaternion& quat)
		{
			this->x -= quat.x;
			this->y -= quat.y;
			this->z -= quat.z;
			this->w -= quat.w;
			return *this;
		}
		inline Quaternion& operator*=(const Quaternion& quat)
		{
			Quaternion temp = *this;

			this->x = temp.w * quat.x + temp.x * quat.w + temp.y * quat.z - temp.z * quat.y;
			this->y = temp.w * quat.y + temp.y * quat.w + temp.z * quat.x - temp.x * quat.z;
			this->z = temp.w * quat.z + temp.z * quat.w + temp.x * quat.y - temp.y * quat.x;
			this->w = temp.w * quat.w - temp.x * quat.x - temp.y * quat.y - temp.z * quat.z;

			return *this;
		}
		inline Quaternion& operator/=(const Quaternion& quat)
		{
			assert(quat != Quaternion::ZERO);
			this->x /= quat.x;
			this->y /= quat.y;
			this->z /= quat.z;
			this->w /= quat.w;
			return *this;
		}
		inline Quaternion& operator*=(const real quat)
		{
			this->x *= quat;
			this->y *= quat;
			this->z *= quat;
			this->w *= quat;
			return *this;
		}
		inline Quaternion& operator/=(const real quat)
		{
			assert(quat != static_cast<real>(0.0));
			this->x /= quat;
			this->y /= quat;
			this->z /= quat;
			this->w /= quat;
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

		static Quaternion FromAngleAxis(real angle, const Vec3& axis)
		{
			real halfAngle = angle * static_cast<real>(0.5);
			real tempSin =
#ifdef DOUBLEPRECISION
				sin(halfAngle);
#else
				sinf(halfAngle);
#endif
			real tempCos =
#ifdef DOUBLEPRECISION
				cos(halfAngle);
#else
				cosf(halfAngle);
#endif

			return Quaternion
			{
				axis.x * tempSin,
				axis.y * tempSin,
				axis.z * tempSin,
				tempCos
			};
		}

		inline bool Equals(const Quaternion& quat) const
		{
			return *this == quat;
		}
		inline bool Equals(const Quaternion& quat, const real tolerance) const
		{
			real dot = this->Dot(quat);
			real angle =
#ifdef DOUBLEPRECISION
				acos(static_cast<real>(2.0) * dot * dot - static_cast<real>(1.0));
#else
				acosf(static_cast<real>(2.0) * dot * dot - static_cast<real>(1.0));
#endif
			return abs(angle) <= tolerance;
		}
	};
}