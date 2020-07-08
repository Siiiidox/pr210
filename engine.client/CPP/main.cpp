
#include <iostream>

#include "application.h"
#include "vec3.h"

int main(int argc, char** argv)
{
	Application app;
	app.Init();
	app.Run();
	app.Shutdown();


	return 0;
}
