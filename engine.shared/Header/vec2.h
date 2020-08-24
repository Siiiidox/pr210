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
		/*!
		*	@brief default X
		*/
		real x = static_cast<real>(0.0);
		/*!
		*	@brief default Y
		*/
		real y = static_cast<real>(0.0);

		/*!
		*	@brief variable ZERO to get a Vector of (0,0);
		*/
		static const Vec2 ZERO;
		/*!
		*	@brief variable UNITX to get a Vector of (1,0);
		*/
		static const Vec2 UNITX;
		/*!
		*	@brief variable UNITY to get a Vector of (0,1);
		*/
		static const Vec2 UNITY;
		/*!
		*	@brief variable UNITSCALE to get a Vector of (1,1);
		*/
		static const Vec2 UNITSCALE;

		inline Vec2(const Vec2& vec)
			: Vec2(vec.x, vec.y)
		{
		}

		/*!
		*	@brief default Constructor
		*/
		inline Vec2(real x = static_cast<real>(0.0)
			, real y = static_cast<real>(0.0)) :
			x(x),
			y(y)
		{
		}

		/*!
		*	@brief normalize the Vector2
		*	@return boolean if the normalized >= MIN
		*/
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

		/*!
		*	@brief Magnitude for Vector2
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
		*	@brief SqrMagnitude of the Vector2
		*	@return real
		*/
		inline real SqrMagnitude() const
		{
			return x * x + y * y;
		}

		/*!
		*	@brief Dot product of the Vector2
		*	@param Vec2
		*	@return real
		*/
		inline real Dot(Vec2 vector) const
		{
			return	(x * vector.x) +
				(y * vector.y);
		}
		/*!
		*	@brief Cross product of the Vector2
		*	@param Vec2
		*	@return real
		*/
		inline real Cross(Vec2 vector) const
		{
			return  x * vector.y - y * vector.x;
		}
		/*!
		*	@brief Lerp for Vector2
		*	@param Vec2
		*	@param real
		*	@return Vec2
		*/
		inline Vec2 Lerp(Vec2 vector, real time) const
		{
			return (*this) + (vector - (*this)) * time;
		}
		/*!
		*	@brief returns the Distance of the Vector2 to this Vector2
		*	@param Vec2
		*	@return real
		*/
		inline real Distance(Vec2 vector) const
		{
#ifdef DOUBLEPRECISION
			return sqrt(SqrDistance(vector));
#else
			return sqrtf(SqrDistance(vector));
#endif
		}
		/*!
		*	@brief returns the SqrDistance of the Vector2 to this Vector2
		*	@param Vec2
		*	@return real
		*/
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