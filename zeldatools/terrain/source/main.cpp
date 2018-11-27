#include "HGHT.h"
#include "MATE.h"
#include <experimental\filesystem>
#include <thread>
namespace fs = std::experimental::filesystem;
#include "OpenGLApplication.h"

#include "TextureHolder.h"

// maximum number of concurrent threads
const int maxThreads = 8;

void dumpHGHT(const std::string& filename)
{
	HGHT hght(filename);

	std::string outputFolder = fs::path(filename).parent_path().string() + "\\images\\" + fs::path(filename).stem().string().substr(1, 1) + "\\";

	if (!fs::exists(outputFolder))
	{
		fs::create_directory(outputFolder);
	}

	hght.writeOBJ(outputFolder + fs::path(filename).stem().string());
}

void dumpMATE(const std::string& filename)
{
	MATE mate(filename);

	std::string outputFolder = fs::path(filename).parent_path().string() + "\\images\\" + fs::path(filename).stem().string().substr(1, 1) + "\\";

	if (!fs::exists(outputFolder))
	{
		fs::create_directory(outputFolder);
	}

	mate.writeRaw(outputFolder + fs::path(filename).stem().string());
}


int main(int argc, char* argv[])
{
	OpenGLApplication app;
	app.run();

    return 0;
}