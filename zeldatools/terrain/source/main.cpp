#include "OpenGLApplication.h"

// maximum number of concurrent threads
const int maxThreads = 8;

int main(int argc, char* argv[])
{
	OpenGLApplication app(1280, 720, "zelda");
	app.run();

    return 0;
}