#include "vec3.h"
#include "matrix4x4.h"


using namespace Engine::Math;


const Vec3 Vec3::ZERO = { static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0) };
const Vec3 Vec3::UNITX = { static_cast<real>(1.0), static_cast<real>(0.0), static_cast<real>(0.0) };
const Vec3 Vec3::UNITY = { static_cast<real>(0.0), static_cast<real>(1.0), static_cast<real>(0.0) };
const Vec3 Vec3::UNITZ = { static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0) };
const Vec3 Vec3::UNITSCALE = { static_cast<real>(1.0), static_cast<real>(1.0), static_cast<real>(1.0) };


Vec3& Vec3::operator*=(const Matrix4x4& matrix)
{
	*this = matrix * *this;
	return *this;
}
