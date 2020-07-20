
#include <iostream>

#include "application.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "mathUtils.h"
#include "quaternion.h"

int main(int argc, char** argv)
{

	Engine::Math::Quaternion q = Engine::Math::Quaternion::FromAngleAxis(static_cast<real>(1), Engine::Math::Vec3::UNITX);

	Engine::Math::Quaternion v = Engine::Math::Quaternion(static_cast<real>(0.0), static_cast<real>(1.0), static_cast<real>(1.0), static_cast<real>(0.0));
	Engine::Math::Quaternion t = q * v * -q;

	Application app;
	app.Init();
	app.Run();
	app.Shutdown();


	return 0;
}
