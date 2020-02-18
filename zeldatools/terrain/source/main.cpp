#include "OpenGLApplication.h"
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <thread>
#include "filenameUtils.h"
#include "HGHT.h"
#include "MATE.h"
#include "GRASS.h"
#include "WATER.h"

#include "TSCB.h"

namespace fs = std::experimental::filesystem;

// maximum number of concurrent threads
const int maxThreads = 8;

void DumpHGHT(const std::string& filename)
{
	HGHT hght(filename);
	hght.writeTexture(filenameUtils::getHGHTFolder() + "/images/" + std::to_string(filenameUtils::getLODFromFilename(filename)) + "/" + filenameUtils::getFileNameOnly(filename) + ".png");
}

void DumpMATE(const std::string& filename)
{
	MATE mate(filename);
	mate.writeTexture(filenameUtils::getMATEFolder() + "/images/" + std::to_string(filenameUtils::getLODFromFilename(filename)) + "/" + filenameUtils::getFileNameOnly(filename) + ".png");
}

void DumpWATER(const std::string& filename)
{
	WATER water(filename);
	water.writeTexture(filenameUtils::getWATERFolder() + "/images/" + std::to_string(filenameUtils::getLODFromFilename(filename)) + "/" + filenameUtils::getFileNameOnly(filename) + ".png");
}

void DumpGRASS(const std::string& filename)
{
	GRASS grass(filename);
	grass.writeTexture(filenameUtils::getGRASSFolder() + "/images/" + std::to_string(filenameUtils::getLODFromFilename(filename)) + "/" + filenameUtils::getFileNameOnly(filename) + ".png");
}

// creates an "images" folder in HGHT, MATE, GRASS, and WATER folders
// creates folders numbered 0-8 within "images" folders
void createDirectories()
{
	// create directorys if they dont exist already
	if (!fs::exists(fs::path(filenameUtils::getHGHTFolder() + "/images/")))
	{
		fs::create_directory(fs::path(filenameUtils::getHGHTFolder() + "/images/"));
	}
	if (!fs::exists(fs::path(filenameUtils::getMATEFolder() + "/images/")))
	{
		fs::create_directory(fs::path(filenameUtils::getMATEFolder() + "/images/"));
	}
	if (!fs::exists(fs::path(filenameUtils::getGRASSFolder() + "/images/")))
	{
		fs::create_directory(fs::path(filenameUtils::getGRASSFolder() + "/images/"));
	}
	if (!fs::exists(fs::path(filenameUtils::getWATERFolder() + "/images/")))
	{
		fs::create_directory(fs::path(filenameUtils::getWATERFolder() + "/images/"));
	}

	// create numbered directories
	for (int i = 0; i < 9; i++)
	{
		if (!fs::exists(fs::path(filenameUtils::getHGHTFolder() + "/images/" + std::to_string(i) + "/")))
		{
			fs::create_directory(fs::path(filenameUtils::getHGHTFolder() + "/images/" + std::to_string(i) + "/"));
		}
		if (!fs::exists(fs::path(filenameUtils::getMATEFolder() + "/images/" + std::to_string(i) + "/")))
		{
			fs::create_directory(fs::path(filenameUtils::getMATEFolder() + "/images/" + std::to_string(i) + "/"));
		}
		if (!fs::exists(fs::path(filenameUtils::getGRASSFolder() + "/images/" + std::to_string(i) + "/")))
		{
			fs::create_directory(fs::path(filenameUtils::getGRASSFolder() + "/images/" + std::to_string(i) + "/"));
		}
		if (!fs::exists(fs::path(filenameUtils::getWATERFolder() + "/images/" + std::to_string(i) + "/")))
		{
			fs::create_directory(fs::path(filenameUtils::getWATERFolder() + "/images/" + std::to_string(i) + "/"));
		}
	}
}

void DumpAll()
{
	// get instance of textureholder
	TextureHolder::getInstance();

	createDirectories();

	// search every file and sort into filenames
	std::vector<std::string> HGHTfilenames;
	std::vector<std::string> MATEfilenames;
	std::vector<std::string> GRASSfilenames;
	std::vector<std::string> WATERfilenames;

	for (auto& p : fs::recursive_directory_iterator(filenameUtils::getRootFolder()))
	{
		if (p.path().has_extension() && p.path().extension().string() != ".png")
		{
			std::string path = p.path().string();

			if (path.find(".hght") != std::string::npos)
				HGHTfilenames.push_back(p.path().string());
			else if (path.find(".mate") != std::string::npos)
				MATEfilenames.push_back(p.path().string());
			else if (path.find(".grass.extm") != std::string::npos)
				GRASSfilenames.push_back(p.path().string());
			else if (path.find(".water.extm") != std::string::npos)
				WATERfilenames.push_back(p.path().string());
		}
	}

	// create threads
	std::thread threads[maxThreads];

	int numDone = 0;
	int numtoDo = 0;

	std::cout << "Doing HGHT files\n";

	// do HGHT files
	while (numDone < HGHTfilenames.size())
	{
		numtoDo = std::min((int)HGHTfilenames.size() - numDone, maxThreads);

		for (int i = 0; i < numtoDo; i++)
		{
			threads[i] = std::thread(DumpHGHT, HGHTfilenames[numDone + i]);
		}
		for (int i = 0; i < numtoDo; i++)
		{
			while (!threads[i].joinable())
			{

			}
			threads[i].join();
		}

		numDone += numtoDo;

		std::cout << numDone << " / " << HGHTfilenames.size() << std::endl;
	}

	numDone = 0;
	std::cout << "Doing MATE files\n";

	// do MATE files
	while (numDone < MATEfilenames.size())
	{
		numtoDo = std::min((int)MATEfilenames.size() - numDone, maxThreads);

		for (int i = 0; i < numtoDo; i++)
		{
			threads[i] = std::thread(DumpMATE, MATEfilenames[numDone + i]);
		}
		for (int i = 0; i < numtoDo; i++)
		{
			while (!threads[i].joinable())
			{

			}
			threads[i].join();
		}

		numDone += numtoDo;

		std::cout << numDone << " / " << MATEfilenames.size() << std::endl;
	}

	numDone = 0;
	std::cout << "Doing GRASS files\n";

	// do GRASS files
	while (numDone < GRASSfilenames.size())
	{
		numtoDo = std::min((int)GRASSfilenames.size() - numDone, maxThreads);

		for (int i = 0; i < numtoDo; i++)
		{
			threads[i] = std::thread(DumpGRASS, GRASSfilenames[numDone + i]);
		}
		for (int i = 0; i < numtoDo; i++)
		{
			while (!threads[i].joinable())
			{

			}
			threads[i].join();
		}

		numDone += numtoDo;

		std::cout << numDone << " / " << GRASSfilenames.size() << std::endl;
	}

	numDone = 0;
	std::cout << "Doing WATER files\n";

	// do WATER files
	while (numDone < WATERfilenames.size())
	{
		numtoDo = std::min((int)WATERfilenames.size() - numDone, maxThreads);

		for (int i = 0; i < numtoDo; i++)
		{
			threads[i] = std::thread(DumpWATER, WATERfilenames[numDone + i]);
		}
		for (int i = 0; i < numtoDo; i++)
		{
			while (!threads[i].joinable())
			{

			}
			threads[i].join();
		}

		numDone += numtoDo;

		std::cout << numDone << " / " << WATERfilenames.size() << std::endl;
	}


	std::cout << "Done\n";
}

int main(int argc, char* argv[])
{
	/*OpenGLApplication app(1280, 720, "zelda");
	app.run();

    return 0;*/

	TSCB::getInstance().writeCombinedOBJ(1, "C:/Users/Andrew/Desktop/51.obj");

	std::cin.get();

	return 0;
}