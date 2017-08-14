#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>
#include "Model.h"
#include "HeightMap.h"
#include "SARC.h"
#pragma comment(linker, "/STACK:20000000")

static const int maximumThreads = 4;

// returns path to the executable
std::string getProgramPath()
{
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

void processFile(std::string& fileName)
{
    //if (fileName.find("hght") != std::string::npos)
    //{
    //    std::cout << "File contains height map data" << std::endl;
    //}
    //else if (fileName.find("mate") != std::string::npos)
    //{
    //    std::cout << "File contains material data" << std::endl;
    //    //Material newMaterial(fileName, shortFileName);
    //}
    //else
    //{
    //    std::cout << "Unknown file type" << std::endl;
    //    system("pause");
    //}
    SARC currentFile(fileName);
}

void unpackFiles()
{
    std::ifstream fileList;
    fileList.open("packedFileNames.txt", std::ios::_Nocreate | std::ios::binary);
    std::vector<std::string>fileNames;

    while (!fileList.eof())
    {
        std::string currentFileName;
        fileList >> currentFileName;
        fileNames.push_back(currentFileName);
    }
    fileList.close();

    int numFiles = fileNames.size();
    int completedFiles = 0;

    std::thread threads[maximumThreads];

    while (completedFiles < numFiles)
    {
        unsigned int numberToDo = numFiles - completedFiles;

        if (numberToDo > maximumThreads)
        {
            numberToDo = maximumThreads;
        }

        for (unsigned int i = 0; i < numberToDo; i++)
        {
            threads[i] = std::thread(processFile, fileNames[completedFiles + i]);
        }
        for (unsigned int i = 0; i < numberToDo; i++)
        {
            threads[i].join();
            completedFiles++;
            std::cout << "Progress: " << completedFiles + i << "/" << numFiles << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    std::cout << "enter a quality level: ";
    int qualityLevel;
    std::cin >> qualityLevel;

    //Model model(qualityLevel);
    HeightMap heightmap(qualityLevel);

    //unpackFiles();
    
    system("pause");
    return 0;
}