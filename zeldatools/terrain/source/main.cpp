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
	// create output folder if it does not already exist
	//if (!fs::exists("C:\\Users\\Andrew\\Desktop\\zelda\\terrain\\mate\\images\\"))
	//{
	//	fs::create_directory("C:\\Users\\Andrew\\Desktop\\zelda\\terrain\\mate\\images\\");
	//}

	//std::vector<std::string> filenames;

	//// count files (probably can be improved)
	//for (auto& iter : fs::directory_iterator("C:\\Users\\Andrew\\Desktop\\zelda\\terrain\\mate\\"))
	//{
	//	if (iter.path().has_extension())
	//	{
	//		filenames.push_back(iter.path().string());
	//	}
	//}

	//TextureHolder::getInstance();

	// create threads
	/*std::thread threads[maxThreads];

	int numFiles = filenames.size();
	int filesToDo;

	for (int completedFiles = 0; completedFiles < filenames.size();)
	{
		filesToDo = std::min(maxThreads, numFiles - completedFiles);

		for (int i = 0; i < filesToDo; i++)
		{
			threads[i] = std::thread(dumpMATE, filenames[completedFiles + i]);
		}
		for (int i = 0; i < filesToDo; i++)
		{
			threads[i].join();
		}

		completedFiles += filesToDo;
	}

	system("pause");*/

	OpenGLApplication app;
	app.run();

    return 0;
}