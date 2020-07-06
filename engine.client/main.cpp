// EXTERNAL INCLUDES
#include <iostream>
// INTERNAL INCLUDES
#include "client.h"
using namespace std;

int main(int argc, char** argv)
{
	Application app;
	app.Init();
	app.Run();
	app.Shutdown();
	return 0;
}
