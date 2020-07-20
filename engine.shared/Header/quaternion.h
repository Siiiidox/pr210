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
			#endif  
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
			return Quaternion{ this->x + quat.x, this->y + quat.y, this->z + quat.z, this->w + quat.w };
		}
		inline Quaternion operator-(const Quaternion& quat) const
		{
			return Quaternion{ this->x - quat.x, this->y - quat.y, this->z - quat.z, this->w - quat.w };
		}
		inline Quaternion operator*(const Quaternion& quat) const
		{
			return Quaternion
			{
				this->w * quat.x + this->x * quat.w + this->y * quat.z - this->z * quat.y,
				this->w * quat.y + this->y * quat.w + this->z * quat.x - this->x * quat.z,
				this->w * quat.z + this->z * quat.w + this->x * quat.y - this->y * quat.x,
				this->w * quat.w - this->x * quat.x - this->y * quat.y - this->z * quat.z
			};

		}
		inline Quaternion operator/(const Quaternion& quat) const
		{
			assert(quat != Quaternion::ZERO);
			return Quaternion{ this->x / quat.x, this->y / quat.y, this->z / quat.z, this->w / quat.w };
		}
		inline Quaternion operator*(const real t) const
		{
			return Quaternion{ this->x * t, this->y * t, this->z * t, this->w * t };
		}
		inline Quaternion operator/(const real t) const
		{
			assert(t != static_cast<real>(0.0));
			return Quaternion{ this->x / t,this->y / t, this->z / t, this->w / t };
		}

		inline Vec3 operator*(const Vec3 vector) const
		{
			Vec3 axis = Vec3(this->x, this->y, this->z);
			Vec3 u = axis.Cross(vector);
			Vec3 v = axis.Cross(u);
			u *= static_cast<real>(2.0)* this->w;
			v *= static_cast<real>(2.0);

			return vector + u + v;
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
			return Quaternion{ -x, -y , -z, w };
		}

		static Quaternion FromAngleAxis(real angle, const Vec3& axis)
		{
			real halfAngle = angle * static_cast<real>(0.5);
		#ifdef DOUBLEPRECISION
			real tempSin = sin(halfAngle);
		#else
			real tempSin = sinf(halfAngle);
		#endif

		#ifdef DOUBLEPRECISION
			real tempCos = cos(halfAngle);
		#else
			real tempCos = cosf(halfAngle);
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
		#ifdef DOUBLEPRECISION
			real angle = acos(static_cast<real>(2.0)* dot* dot - static_cast<real>(1.0));
		#else
			real angle = acosf(static_cast<real>(2.0)* dot* dot - static_cast<real>(1.0));
		#endif
			return abs(angle) <= tolerance;
		}

		inline void ToAngleAxis(real& angle, Vec3& axis) const
		{
			if (this->SqrMagnitude() > 0)
			{
				angle = static_cast<real>(2.0)* acos(this->w);
				axis *= (static_cast<real>(1.0) - this->Magnitude());
			}
			else
			{
				angle = static_cast<real>(0.0);
				axis = Vec3::UNITX;
			}

		}

		inline Quaternion Inverse() const
		{
			if (this->SqrMagnitude() > 0)
			{
				real inverse = static_cast<real>(1.0) / this->Magnitude();
				return Quaternion
				{
					x * inverse,
					y * inverse,
					z * inverse,
					w * inverse
				};
			}
			else
			{
				return Quaternion::ZERO;
			}
		}
	};
}