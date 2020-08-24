
#include <iostream>

#include "application.h"


int main(int argc, char** argv)
{
	//Start of the Programm and starts the EngineApplication
	Application app;
	app.Init();
	app.Run();
	app.Shutdown();


	return 0;
}
