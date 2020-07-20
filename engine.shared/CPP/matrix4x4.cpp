#include "matrix4x4.h"

using namespace Engine::Math;

const Matrix4x4 Matrix4x4::IDENTITY = 
{
	static_cast<real>(1.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(1.0), static_cast<real>(0.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)
};

const Matrix4x4 Matrix4x4::ZERO =
{
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0),
	static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0)
};