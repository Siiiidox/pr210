#pragma once

#include <cmath>
#include <cfloat>
#include <cassert>

#include "types.h"
#include "vec3.h"
#include "mathUtils.h"
#include "quaternion.h"

namespace Engine::Math
{

	class Matrix4x4
	{
	public:
		real m11 = static_cast<real>(0.0), m12 = static_cast<real>(0.0), m13 = static_cast<real>(0.0), m14 = static_cast<real>(0.0);
		real m21 = static_cast<real>(0.0), m22 = static_cast<real>(0.0), m23 = static_cast<real>(0.0), m24 = static_cast<real>(0.0);
		real m31 = static_cast<real>(0.0), m32 = static_cast<real>(0.0), m33 = static_cast<real>(0.0), m34 = static_cast<real>(0.0);
		real m41 = static_cast<real>(0.0), m42 = static_cast<real>(0.0), m43 = static_cast<real>(0.0), m44 = static_cast<real>(0.0);

		static const Matrix4x4 IDENTITY;
		static const Matrix4x4 ZERO;


		inline Matrix4x4 Transpose() const
		{
			return Matrix4x4
			{
				this->m11, this->m21, this->m31, this->m41,
				this->m12, this->m22, this->m32, this->m42,
				this->m13, this->m23, this->m33, this->m43,
				this->m14, this->m24, this->m34, this->m44
			};
		}

		inline Matrix4x4& operator+=(const Matrix4x4& matrix)
		{
			this->m11 += matrix.m11;
			this->m12 += matrix.m12;
			this->m13 += matrix.m13;
			this->m14 += matrix.m14;

			this->m21 += matrix.m21;
			this->m22 += matrix.m22;
			this->m23 += matrix.m23;
			this->m24 += matrix.m24;

			this->m31 += matrix.m31;
			this->m32 += matrix.m32;
			this->m33 += matrix.m33;
			this->m34 += matrix.m34;

			this->m41 += matrix.m41;
			this->m42 += matrix.m42;
			this->m43 += matrix.m43;
			this->m44 += matrix.m44;

			return *this;
		}

		inline Matrix4x4& operator-=(const Matrix4x4& matrix)
		{
			this->m11 -= matrix.m11;
			this->m12 -= matrix.m12;
			this->m13 -= matrix.m13;
			this->m14 -= matrix.m14;

			this->m21 -= matrix.m21;
			this->m22 -= matrix.m22;
			this->m23 -= matrix.m23;
			this->m24 -= matrix.m24;

			this->m31 -= matrix.m31;
			this->m32 -= matrix.m32;
			this->m33 -= matrix.m33;
			this->m34 -= matrix.m34;

			this->m41 -= matrix.m41;
			this->m42 -= matrix.m42;
			this->m43 -= matrix.m43;
			this->m44 -= matrix.m44;

			return *this;
		}

		inline Matrix4x4& operator*=(const Matrix4x4& matrix)
		{
			Matrix4x4 temp = *this;

			this->m11 = temp.m11 * matrix.m11 + temp.m12 * matrix.m21 + temp.m13 * matrix.m31 + temp.m14 * matrix.m41;
			this->m12 = temp.m11 * matrix.m12 + temp.m12 * matrix.m22 + temp.m13 * matrix.m32 + temp.m14 * matrix.m42;
			this->m13 = temp.m11 * matrix.m13 + temp.m12 * matrix.m23 + temp.m13 * matrix.m33 + temp.m14 * matrix.m43;
			this->m14 = temp.m11 * matrix.m14 + temp.m12 * matrix.m24 + temp.m13 * matrix.m34 + temp.m14 * matrix.m44;

			this->m21 = temp.m21 * matrix.m11 + temp.m22 * matrix.m21 + temp.m23 * matrix.m31 + temp.m24 * matrix.m41;
			this->m22 = temp.m21 * matrix.m12 + temp.m22 * matrix.m22 + temp.m23 * matrix.m32 + temp.m24 * matrix.m42;
			this->m23 = temp.m21 * matrix.m13 + temp.m22 * matrix.m23 + temp.m23 * matrix.m33 + temp.m24 * matrix.m43;
			this->m24 = temp.m21 * matrix.m14 + temp.m22 * matrix.m24 + temp.m23 * matrix.m34 + temp.m24 * matrix.m44;

			this->m31 = temp.m31 * matrix.m11 + temp.m32 * matrix.m21 + temp.m33 * matrix.m31 + temp.m34 * matrix.m41;
			this->m32 = temp.m31 * matrix.m12 + temp.m32 * matrix.m22 + temp.m33 * matrix.m32 + temp.m34 * matrix.m42;
			this->m33 = temp.m31 * matrix.m13 + temp.m32 * matrix.m23 + temp.m33 * matrix.m33 + temp.m34 * matrix.m43;
			this->m34 = temp.m31 * matrix.m14 + temp.m32 * matrix.m24 + temp.m33 * matrix.m34 + temp.m34 * matrix.m44;

			this->m41 = temp.m41 * matrix.m11 + temp.m42 * matrix.m21 + temp.m43 * matrix.m31 + temp.m44 * matrix.m41;
			this->m42 = temp.m41 * matrix.m12 + temp.m42 * matrix.m22 + temp.m43 * matrix.m32 + temp.m44 * matrix.m42;
			this->m43 = temp.m41 * matrix.m13 + temp.m42 * matrix.m23 + temp.m43 * matrix.m33 + temp.m44 * matrix.m43;
			this->m44 = temp.m41 * matrix.m14 + temp.m42 * matrix.m24 + temp.m43 * matrix.m34 + temp.m44 * matrix.m44;

			return *this;
		}

		inline Matrix4x4 operator+(const Matrix4x4& matrix) const
		{
			return Matrix4x4
			{
				this->m11 + matrix.m11,
				this->m12 + matrix.m12,
				this->m13 + matrix.m13,
				this->m14 + matrix.m14,

				this->m21 + matrix.m21,
				this->m22 + matrix.m22,
				this->m23 + matrix.m23,
				this->m24 + matrix.m24,

				this->m31 + matrix.m31,
				this->m32 + matrix.m32,
				this->m33 + matrix.m33,
				this->m34 + matrix.m34,

				this->m41 + matrix.m41,
				this->m42 + matrix.m42,
				this->m43 + matrix.m43,
				this->m44 + matrix.m44
			};
		}

		inline Matrix4x4 operator-(const Matrix4x4& matrix) const
		{
			return Matrix4x4
			{
				this->m11 - matrix.m11,
				this->m12 - matrix.m12,
				this->m13 - matrix.m13,
				this->m14 - matrix.m14,

				this->m21 - matrix.m21,
				this->m22 - matrix.m22,
				this->m23 - matrix.m23,
				this->m24 - matrix.m24,

				this->m31 - matrix.m31,
				this->m32 - matrix.m32,
				this->m33 - matrix.m33,
				this->m34 - matrix.m34,

				this->m41 - matrix.m41,
				this->m42 - matrix.m42,
				this->m43 - matrix.m43,
				this->m44 - matrix.m44
			};
		}

		inline Matrix4x4 operator*(const Matrix4x4& matrix) const
		{
			return Matrix4x4
			{
			this->m11 * matrix.m11 + this->m12 * matrix.m21 + this->m13 * matrix.m31 + this->m14 * matrix.m41,
			this->m11 * matrix.m12 + this->m12 * matrix.m22 + this->m13 * matrix.m32 + this->m14 * matrix.m42,
			this->m11 * matrix.m13 + this->m12 * matrix.m23 + this->m13 * matrix.m33 + this->m14 * matrix.m43,
			this->m11 * matrix.m14 + this->m12 * matrix.m24 + this->m13 * matrix.m34 + this->m14 * matrix.m44,

			this->m21 * matrix.m11 + this->m22 * matrix.m21 + this->m23 * matrix.m31 + this->m24 * matrix.m41,
			this->m21 * matrix.m12 + this->m22 * matrix.m22 + this->m23 * matrix.m32 + this->m24 * matrix.m42,
			this->m21 * matrix.m13 + this->m22 * matrix.m23 + this->m23 * matrix.m33 + this->m24 * matrix.m43,
			this->m21 * matrix.m14 + this->m22 * matrix.m24 + this->m23 * matrix.m34 + this->m24 * matrix.m44,

			this->m31 * matrix.m11 + this->m32 * matrix.m21 + this->m33 * matrix.m31 + this->m34 * matrix.m41,
			this->m31 * matrix.m12 + this->m32 * matrix.m22 + this->m33 * matrix.m32 + this->m34 * matrix.m42,
			this->m31 * matrix.m13 + this->m32 * matrix.m23 + this->m33 * matrix.m33 + this->m34 * matrix.m43,
			this->m31 * matrix.m14 + this->m32 * matrix.m24 + this->m33 * matrix.m34 + this->m34 * matrix.m44,

			this->m41 * matrix.m11 + this->m42 * matrix.m21 + this->m43 * matrix.m31 + this->m44 * matrix.m41,
			this->m41 * matrix.m12 + this->m42 * matrix.m22 + this->m43 * matrix.m32 + this->m44 * matrix.m42,
			this->m41 * matrix.m13 + this->m42 * matrix.m23 + this->m43 * matrix.m33 + this->m44 * matrix.m43,
			this->m41 * matrix.m14 + this->m42 * matrix.m24 + this->m43 * matrix.m34 + this->m44 * matrix.m44
			};
		}



		inline bool operator==(const Matrix4x4& matrix) const
		{
			if (this->m11 == matrix.m11
				&& this->m12 == matrix.m12
				&& this->m13 == matrix.m13
				&& this->m14 == matrix.m14

				&& this->m21 == matrix.m21
				&& this->m22 == matrix.m22
				&& this->m23 == matrix.m23
				&& this->m24 == matrix.m24

				&& this->m31 == matrix.m31
				&& this->m32 == matrix.m32
				&& this->m33 == matrix.m33
				&& this->m34 == matrix.m34

				&& this->m41 == matrix.m41
				&& this->m42 == matrix.m42
				&& this->m43 == matrix.m43
				&& this->m44 == matrix.m44)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool operator!=(const Matrix4x4& matrix) const
		{
			return !(*this == matrix);
		}

		//Vec3

		inline Vec3 operator*(const Vec3& vector) const
		{
			return Vec3
			{
				vector.x * this->m11 + vector.y * this->m12 + vector.z * this->m13,
				vector.x * this->m21 + vector.y * this->m22 + vector.z * this->m23,
				vector.x * this->m31 + vector.y * this->m32 + vector.z * this->m33
			};
		}

		//Real

		inline Matrix4x4& operator*=(const real& real)
		{
			this->m11 *= real;
			this->m12 *= real;
			this->m13 *= real;
			this->m14 *= real;

			this->m21 *= real;
			this->m22 *= real;
			this->m23 *= real;
			this->m24 *= real;

			this->m31 *= real;
			this->m32 *= real;
			this->m33 *= real;
			this->m34 *= real;

			this->m41 *= real;
			this->m42 *= real;
			this->m43 *= real;
			this->m44 *= real;
		}

		inline Matrix4x4 operator*(const real& real) const
		{
			return Matrix4x4
			{
				this->m11 * real,
				this->m12 * real,
				this->m13 * real,
				this->m14 * real,

				this->m21 * real,
				this->m22 * real,
				this->m23 * real,
				this->m24 * real,

				this->m31 * real,
				this->m32 * real,
				this->m33 * real,
				this->m34 * real,

				this->m41 * real,
				this->m42 * real,
				this->m43 * real,
				this->m44 * real
			};
		}


		static Matrix4x4 FromTranslation(Vec3 vector)
		{
			Matrix4x4 matrix = Matrix4x4::IDENTITY;
			matrix.m14 = vector.x;
			matrix.m24 = vector.y;
			matrix.m34 = vector.z;
			return matrix;
		}

		static Matrix4x4 FromScale(Vec3 vector = Vec3::UNITSCALE)
		{
			return Matrix4x4
			{
				vector.x, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), vector.y, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), vector.z, static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
			};
		}

		static Matrix4x4 FromOrientation(Quaternion quat)
		{
			Matrix4x4 matrix = Matrix4x4::IDENTITY;
			real tx = quat.x + quat.x;
			real ty = quat.y + quat.y;
			real tz = quat.z + quat.z;

			real twx = quat.w * tx;
			real twy = quat.w * ty;
			real twz = quat.w * tz;

			real txx = quat.x * tx;
			real txy = quat.x * ty;
			real txz = quat.x * tz;

			real tyy = quat.y * ty;
			real tyz = quat.y * tz;

			real tzz = quat.z * tz;

			matrix.m11 = static_cast<real>(1.0) - (tyy + tzz);
			matrix.m12 = txy - twz;
			matrix.m13 = txz + twy;

			matrix.m21 = txy + twz;
			matrix.m22 = static_cast<real>(1.0) - (txx + tzz);
			matrix.m23 = tyz - twx;

			matrix.m31 = txz - twy;
			matrix.m32 = tyz + twx;
			matrix.m33 = static_cast<real>(1.0) - (txx + tyy);

			return matrix;
		}

		static Matrix4x4 FromView(Vec3 vector)
		{
			Vec3 axisX = Vec3::UNITX;
			Vec3 axisY = Vec3::UNITY;
			Vec3 axisZ = Vec3::UNITZ;

			return Matrix4x4
			{
				axisX.x, axisY.x, axisZ.x, -axisX.Dot(vector),
				axisX.y, axisY.y, axisZ.y, -axisY.Dot(vector),
				axisX.z, axisY.z, axisZ.z, -axisZ.Dot(vector),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
			};

		}

		static Matrix4x4 FromOrthographic(real width, real height, real nearPlane = static_cast<real>(0.001), real farPlane = static_cast<real>(10000.0))
		{
			return Matrix4x4
			{
				static_cast<real>(2.0) / width, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(2.0) / height, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0) / (nearPlane - farPlane), nearPlane / (nearPlane - farPlane),
				static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
			};
		}

		static Matrix4x4 FromPerspectiveFOV(real fov = static_cast<real>(75.0), real aspect = static_cast<real>(1.77778), real nearPlane = static_cast<real>(0.001), real farPlane = static_cast<real>(10000.0))
		{
		#ifdef DOUBLEPRECISION
			real scaleY = static_cast<real>(1.0) / tan(fov / static_cast<real>(2.0)* ANGLETORAD());
		#else
			real scaleY = static_cast<real>(1.0) / tanf(fov / static_cast<real>(2.0)* ANGLETORAD());
		#endif
			real scaleX = scaleY / aspect;
			return Matrix4x4
			{
				scaleX, static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), scaleY, static_cast<real>(0.0), static_cast<real>(0.0),
				static_cast<real>(0.0), static_cast<real>(0.0), farPlane / (nearPlane - farPlane), nearPlane* farPlane / (nearPlane - farPlane),
				static_cast<real>(0.0), static_cast<real>(0.0), -1, static_cast<real>(0.0)
			};
		}
	};
}