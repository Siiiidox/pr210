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
		/*!
		*	@brief default X
		*/
		real x = static_cast<real>(0.0);
		/*!
		*	@brief default Y 
		*/
		real y = static_cast<real>(0.0);
		/*!
		*	@brief default Z
		*/
		real z = static_cast<real>(0.0);

		/*!
		*	@brief variable ZERO to get a Vector of (0,0,0);
		*/
		static const Vec3 ZERO;
		/*!
		*	@brief variable UNITX to get a Vector of (1,0,0);
		*/
		static const Vec3 UNITX;
		/*!
		*	@brief variable UNITY to get a Vector of (0,1,0);
		*/
		static const Vec3 UNITY;
		/*!
		*	@brief variable UNITZ to get a Vector of (0,0,1);
		*/
		static const Vec3 UNITZ;
		/*!
		*	@brief variable UNITSCALE to get a Vector of (1,1,1);
		*/
		static const Vec3 UNITSCALE;

		/*!
		*	@brief default Constructor
		*/
		Vec3(real x = static_cast<real>(0.0), real y = static_cast<real>(0.0), real z = static_cast<real>(0.0)) :
			x(x),
			y(y),
			z(z)
		{
		}

		/*!
		*	@brief normalize the Vector3
		*	@return boolean if the normalized >= MIN
		*/
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

		/*!
		*	@brief Magnitude for Vector3
		*	@return real
		*/
		inline real Magnitude() const
		{
		#ifdef DOUBLEPRECISION
			return sqrt(SqrMagnitude());
		#else
			return sqrtf(SqrMagnitude());
		#endif
		}

		/*!
		*	@brief SqrMagnitude of the Vector3
		*	@return real
		*/
		inline real SqrMagnitude() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		/*!
		*	@brief Dot product of the Vector3
		*	@param Vec3
		*	@return real
		*/
		inline real Dot(Vec3 vector) const
		{
			return (x * vector.x) + (y * vector.y) + (z * vector.z);
		}
		/*!
		*	@brief Cross product of the Vector3
		*	@param Vec3
		*	@return Vec3
		*/
		inline Vec3 Cross(Vec3 vector) const
		{
			return Vec3{ (y * vector.z - z * vector.y), (z * vector.x - x * vector.z), (x * vector.y - y * vector.x) };
		}
		/*!
		*	@brief Lerp for Vector3
		*	@param Vec3
		*	@param real
		*	@return Vec3
		*/
		inline Vec3 Lerp(Vec3 vector, real time) const
		{
			return (*this + (vector - *this) * time);
		}
		/*!
		*	@brief returns the Distance of the Vector3 to this Vector3
		*	@param Vec3
		*	@return real
		*/
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