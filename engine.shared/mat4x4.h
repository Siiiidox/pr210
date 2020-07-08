#pragma once

// EXTERNAL INCLUDES
#include <cmath>
#include <cfloat>
#include <cassert>
// INTERNAL INCLUDES
#include "types.h"
#include "vec3.h"
#include "mathutils.h"

namespace Engine::Math
{
	class Mat4x4
	{
	public:
		real m11 = static_cast<real>(0.0), m12 = static_cast<real>(0.0), m13 = static_cast<real>(0.0), m14 = static_cast<real>(0.0);
		real m21 = static_cast<real>(0.0), m22 = static_cast<real>(0.0), m23 = static_cast<real>(0.0), m24 = static_cast<real>(0.0);
		real m31 = static_cast<real>(0.0), m32 = static_cast<real>(0.0), m33 = static_cast<real>(0.0), m34 = static_cast<real>(0.0);
		real m41 = static_cast<real>(0.0), m42 = static_cast<real>(0.0), m43 = static_cast<real>(0.0), m44 = static_cast<real>(0.0);

		static const Mat4x4 Identity;
		static const Mat4x4 Zero;

		inline Mat4x4 Transpose() const
		{
			return Mat4x4
			{
				this->m11, this->m21, this->m31, this->m41,
				this->m12, this->m22, this->m32, this->m42,
				this->m13, this->m23, this->m33, this->m43,
				this->m14, this->m24, this->m34, this->m44
			};
		}

		inline Mat4x4& operator+=(const Mat4x4& rhs)
		{
			this->m11 += rhs.m11;
			this->m12 += rhs.m12;
			this->m13 += rhs.m13;
			this->m14 += rhs.m14;

			this->m21 += rhs.m21;
			this->m22 += rhs.m22;
			this->m23 += rhs.m23;
			this->m24 += rhs.m24;

			this->m31 += rhs.m31;
			this->m32 += rhs.m32;
			this->m33 += rhs.m33;
			this->m34 += rhs.m34;

			this->m41 += rhs.m41;
			this->m42 += rhs.m42;
			this->m43 += rhs.m43;
			this->m44 += rhs.m44;
			return *this;
		}
		inline Mat4x4& operator-=(const Mat4x4& rhs)
		{
			this->m11 -= rhs.m11;
			this->m12 -= rhs.m12;
			this->m13 -= rhs.m13;
			this->m14 -= rhs.m14;
					  
			this->m21 -= rhs.m21;
			this->m22 -= rhs.m22;
			this->m23 -= rhs.m23;
			this->m24 -= rhs.m24;
					  
			this->m31 -= rhs.m31;
			this->m32 -= rhs.m32;
			this->m33 -= rhs.m33;
			this->m34 -= rhs.m34;
					  
			this->m41 -= rhs.m41;
			this->m42 -= rhs.m42;
			this->m43 -= rhs.m43;
			this->m44 -= rhs.m44;
			return *this;
		}
		inline Mat4x4& operator*=(const Mat4x4& rhs)
		{
			this->m11 = this->m11 * rhs.m11 + this->m12 * rhs.m21 + this->m13 * rhs.m31 + this->m14 * rhs.m41;
			this->m12 = this->m11 * rhs.m12 + this->m12 * rhs.m22 + this->m13 * rhs.m32 + this->m14 * rhs.m42;
			this->m13 = this->m11 * rhs.m13 + this->m12 * rhs.m23 + this->m13 * rhs.m33 + this->m14 * rhs.m43;
			this->m14 = this->m11 * rhs.m14 + this->m12 * rhs.m24 + this->m13 * rhs.m34 + this->m14 * rhs.m44;

			this->m21 = this->m21 * rhs.m11 + this->m22 * rhs.m21 + this->m23 * rhs.m31 + this->m24 * rhs.m41;
			this->m22 = this->m21 * rhs.m12 + this->m22 * rhs.m22 + this->m23 * rhs.m32 + this->m24 * rhs.m42;
			this->m23 = this->m21 * rhs.m13 + this->m22 * rhs.m23 + this->m23 * rhs.m33 + this->m24 * rhs.m43;
			this->m24 = this->m21 * rhs.m14 + this->m22 * rhs.m24 + this->m23 * rhs.m34 + this->m24 * rhs.m44;

			this->m31 = this->m31 * rhs.m11 + this->m32 * rhs.m21 + this->m33 * rhs.m31 + this->m34 * rhs.m41;
			this->m32 = this->m31 * rhs.m12 + this->m32 * rhs.m22 + this->m33 * rhs.m32 + this->m34 * rhs.m42;
			this->m33 = this->m31 * rhs.m13 + this->m32 * rhs.m23 + this->m33 * rhs.m33 + this->m34 * rhs.m43;
			this->m34 = this->m31 * rhs.m14 + this->m32 * rhs.m24 + this->m33 * rhs.m34 + this->m34 * rhs.m44;

			this->m41 = this->m41 * rhs.m11 + this->m42 * rhs.m21 + this->m43 * rhs.m31 + this->m44 * rhs.m41;
			this->m42 = this->m41 * rhs.m12 + this->m42 * rhs.m22 + this->m43 * rhs.m32 + this->m44 * rhs.m42;
			this->m43 = this->m41 * rhs.m13 + this->m42 * rhs.m23 + this->m43 * rhs.m33 + this->m44 * rhs.m43;
			this->m44 = this->m41 * rhs.m14 + this->m42 * rhs.m24 + this->m43 * rhs.m34 + this->m44 * rhs.m44;
			return *this;
		}
		inline Mat4x4& operator*=(const real& rhs)
		{
			this->m11 *= rhs;
			this->m12 *= rhs;
			this->m13 *= rhs;
			this->m14 *= rhs;
					  
			this->m21 *= rhs;
			this->m22 *= rhs;
			this->m23 *= rhs;
			this->m24 *= rhs;
					  
			this->m31 *= rhs;
			this->m32 *= rhs;
			this->m33 *= rhs;
			this->m34 *= rhs;
					  
			this->m41 *= rhs;
			this->m42 *= rhs;
			this->m43 *= rhs;
			this->m44 *= rhs;
		}
		
		inline Mat4x4 operator+(const Mat4x4& v) const
		{
			return Mat4x4
			{
				this->m11 + v.m11,
				this->m12 + v.m12,
				this->m13 + v.m13,
				this->m14 + v.m14,
						  
				this->m21 + v.m21,
				this->m22 + v.m22,
				this->m23 + v.m23,
				this->m24 + v.m24,
						  
				this->m31 + v.m31,
				this->m32 + v.m32,
				this->m33 + v.m33,
				this->m34 + v.m34,
						  
				this->m41 + v.m41,
				this->m42 + v.m42,
				this->m43 + v.m43,
				this->m44 + v.m44
			};
		}
		inline Mat4x4 operator-(const Mat4x4& v) const
		{
			return Mat4x4
			{
				this->m11 - v.m11,
				this->m12 - v.m12,
				this->m13 - v.m13,
				this->m14 - v.m14,
						  
				this->m21 - v.m21,
				this->m22 - v.m22,
				this->m23 - v.m23,
				this->m24 - v.m24,
						  
				this->m31 - v.m31,
				this->m32 - v.m32,
				this->m33 - v.m33,
				this->m34 - v.m34,
						  
				this->m41 - v.m41,
				this->m42 - v.m42,
				this->m43 - v.m43,
				this->m44 - v.m44
			};
		}
		inline Mat4x4 operator*(const Mat4x4& rhs) const
		{
			return Mat4x4
			{
				this->m11 * rhs.m11 + this->m12 * rhs.m21 + this->m13 * rhs.m31 + this->m14 * rhs.m41,
				this->m11 * rhs.m12 + this->m12 * rhs.m22 + this->m13 * rhs.m32 + this->m14 * rhs.m42,
				this->m11 * rhs.m13 + this->m12 * rhs.m23 + this->m13 * rhs.m33 + this->m14 * rhs.m43,
				this->m11 * rhs.m14 + this->m12 * rhs.m24 + this->m13 * rhs.m34 + this->m14 * rhs.m44,

				this->m21 * rhs.m11 + this->m22 * rhs.m21 + this->m23 * rhs.m31 + this->m24 * rhs.m41,
				this->m21 * rhs.m12 + this->m22 * rhs.m22 + this->m23 * rhs.m32 + this->m24 * rhs.m42,
				this->m21 * rhs.m13 + this->m22 * rhs.m23 + this->m23 * rhs.m33 + this->m24 * rhs.m43,
				this->m21 * rhs.m14 + this->m22 * rhs.m24 + this->m23 * rhs.m34 + this->m24 * rhs.m44,

				this->m31 * rhs.m11 + this->m32 * rhs.m21 + this->m33 * rhs.m31 + this->m34 * rhs.m41,
				this->m31 * rhs.m12 + this->m32 * rhs.m22 + this->m33 * rhs.m32 + this->m34 * rhs.m42,
				this->m31 * rhs.m13 + this->m32 * rhs.m23 + this->m33 * rhs.m33 + this->m34 * rhs.m43,
				this->m31 * rhs.m14 + this->m32 * rhs.m24 + this->m33 * rhs.m34 + this->m34 * rhs.m44,

				this->m41 * rhs.m11 + this->m42 * rhs.m21 + this->m43 * rhs.m31 + this->m44 * rhs.m41,
				this->m41 * rhs.m12 + this->m42 * rhs.m22 + this->m43 * rhs.m32 + this->m44 * rhs.m42,
				this->m41 * rhs.m13 + this->m42 * rhs.m23 + this->m43 * rhs.m33 + this->m44 * rhs.m43,
				this->m41 * rhs.m14 + this->m42 * rhs.m24 + this->m43 * rhs.m34 + this->m44 * rhs.m44
			};
		}
		inline Vec3 operator*(const Vec3& v) const
		{
			return Vec3
			{
				v.x * this->m11 + v.y * this->m12 + v.z * this->m13,
				v.x * this->m21 + v.y * this->m22 + v.z * this->m23,
				v.x * this->m31 + v.y * this->m32 + v.z * this->m33
			};
		}
		inline Mat4x4 operator*(const real& v) const
		{
			return Mat4x4
			{
				this->m11 * v,
				this->m12 * v,
				this->m13 * v,
				this->m14 * v,
							
				this->m21 * v,
				this->m22 * v,
				this->m23 * v,
				this->m24 * v,
							
				this->m31 * v,
				this->m32 * v,
				this->m33 * v,
				this->m34 * v,
							
				this->m41 * v,
				this->m42 * v,
				this->m43 * v,
				this->m44 * v
			};
		}
		
		inline bool operator==(const Mat4x4& v) const
		{
			if (this->m11 == v.m11
				&& this->m12 == v.m12
				&& this->m13 == v.m13
				&& this->m14 == v.m14

				&& this->m21 == v.m21
				&& this->m22 == v.m22
				&& this->m23 == v.m23
				&& this->m24 == v.m24

				&& this->m31 == v.m31
				&& this->m32 == v.m32
				&& this->m33 == v.m33
				&& this->m34 == v.m34

				&& this->m41 == v.m41
				&& this->m42 == v.m42
				&& this->m43 == v.m43
				&& this->m44 == v.m44)
				return true;
			return false;
		}
		inline bool operator!=(const Mat4x4& v) const
		{
			return !(*this == v);
		}


		static Mat4x4 FromTranslation(Vec3 v)
		{
			Mat4x4 m = Mat4x4::Identity;
			m.m14 = v.x;
			m.m24 = v.y;
			m.m34 = v.z;
			return m;
		}
		static Mat4x4 FromScale(Vec3 v = Vec3::UnitScale)
		{
			return Mat4x4
			{
				v.x, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), v.y, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), v.z, static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0),
			};
		}
		//static Mat4x4 FromOrientation(Quaternion q)
		//{
		//
		//}
		static Mat4x4 FromView(Vec3 v)
		{
			Vec3 axisX = Vec3::UnitX;
			Vec3 axisY = Vec3::UnitY;
			Vec3 axisZ = Vec3::UnitZ;

			return Mat4x4
			{
				axisX.x, axisY.x, axisZ.x, -axisX.Dot(v),
				axisX.y, axisY.y, axisZ.y, -axisY.Dot(v),
				axisX.z, axisY.z, axisZ.z, -axisZ.Dot(v),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
			};
		}
		static Mat4x4 FromOrthographic(real width, real height, real nearPlane = static_cast<real>(0.001), real farPlane = static_cast<real>(10000.0))
		{
			return Mat4x4
			{
				static_cast<real>(2.0) / width, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(2.0) / height, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0) / (nearPlane - farPlane), nearPlane / (nearPlane - farPlane),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
			};
		}
		static Mat4x4 FromPerspectiveFOV(real fov = static_cast<real>(75.0), real aspect = static_cast<real>(1.77778), real nearPlane = static_cast<real>(0.001), real farPlane = static_cast<real>(10000.0))
		{
#ifdef DOUBLEPRECISION
			real S = static_cast<real>(1.0) / tan(fov / static_cast<real>(2.0) * AngleToRad());
#else
			real S = static_cast<real>(1.0) / tanf(fov / static_cast<real>(2.0) * AngleToRad());
#endif
			return Mat4x4
			{
				S, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), S, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), farPlane / (nearPlane - farPlane), nearPlane * farPlane / (nearPlane - farPlane),
				static_cast<real>(0.0), static_cast<real>(0.0), -1, static_cast<real>(0.0)
			};
		}
	};
}