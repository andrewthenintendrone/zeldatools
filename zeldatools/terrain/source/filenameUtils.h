#pragma once
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;


class filenameUtils
{
public:

	// returns the file extension of the file name (e.g ".png")
	static std::string getFileExtension(const std::string& filename)
	{
		return fs::path(filename).extension().string();
	}

	// returns the directory of the file name (e.g "C:\Users\***\Documents\")
	static std::string getFileDirectory(const std::string& filename)
	{
		return fs::path(filename).parent_path().string();
	}

	// returns the file name without the directory (e.g "document.docx")
	static std::string getFilenameNoDirectory(const std::string& filename)
	{
		return fs::path(filename).filename().string();
	}

	// returns the file name without the extension (e.g "C:\Users\***\Documents\document")
	static std::string getFilenameNoExtension(const std::string& filename)
	{
		return fs::path(filename).parent_path().string() + "\\" + fs::path(filename).stem().string();
	}

	// returns the file name without the extension or directory (e.g "document")
	static std::string getFileNameOnly(const std::string& filename)
	{
		return fs::path(filename).stem().string();
	}
	
	static std::string getRootFolder()
	{
		return std::string("C:\\Users\\s170837\\Desktop\\z");
	}

	static std::string getHGHTFolder()
	{
		return getRootFolder() + "\\hght";
	}

	static std::string getMATEFolder()
	{
		return getRootFolder() + "\\mate";
	}

	static std::string getGRASSFolder()
	{
		return getRootFolder() + "\\grass";
	}

	static std::string getWATERFolder()
	{
		return getRootFolder() + "\\water";
	}

	static std::string getRandomFile(const std::string& folder)
	{
		std::vector<std::string> files;

		for (auto& iter : fs::directory_iterator(folder))
		{
			files.push_back(iter.path().string());
		}

		return files[rand() % files.size()];
	}
};

