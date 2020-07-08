
#include <iostream>

#include "application.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "mathUtils.h"

int main(int argc, char** argv)
{

	int t = Engine::Math::CLAMP(4,0,3);

	Application app;
	app.Init();
	app.Run();
	app.Shutdown();


	return 0;
}
