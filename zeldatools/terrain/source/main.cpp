#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "fileNames.h"
#include "Model.h"
#include "HeightMap.h"
#include "Material.h"
#include <vector>
#include <thread>
#include "tscb.h"
#include <thread>
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

char charToValue(char input)
{
    if (input >= '0' && input <= '9')
    {
        return input - '0';
    }
    else if (input >= 'a' && input <= 'z')
    {
        return input - 'a' + (char)10;
    }
    else if (input >= 'A' && input <= 'Z')
    {
        return input - 'A' + (char)10;
    }
    else
    {
        std::cout << "invalid char passed\n";
        return 'a';
    }
}

void processFile(std::string& fileName)
{
    //system("cls");
    //std::cout << "completed: " << i / (float)numNames * 100 << " %";
    std::string shortFileName = fileName.substr(fileName.length() - 22, 10);

    if (fileName.find("hght") != std::string::npos)
    {
        std::cout << "File contains height map data" << std::endl;
        HeightMap newHeightMap(fileName, shortFileName);
        newHeightMap.getHighestPoint();
    }
    else if (fileName.find("mate") != std::string::npos)
    {
        std::cout << "File contains material data" << std::endl;
        Material newMaterial(fileName, shortFileName);
    }
    else
    {
        std::cout << "Unknown file type" << std::endl;
        system("pause");
    }
}

int main(int argc, char* argv[])
{
    TSCB tscb;

    /*std::thread threads[maximumThreads];
    int completedFiles = 1;

    while (completedFiles < argc)
    {
        unsigned int numberToDo = argc - completedFiles;
        if (numberToDo > maximumThreads)
        {
            numberToDo = maximumThreads;
        }

        for (unsigned int i = 0; i < numberToDo; i++)
        {
            threads[i] = std::thread(processFile, std::string(argv[completedFiles + i]));
            system("cls");
        }
        for (unsigned int i = 0; i < numberToDo; i++)
        {
            threads[i].join();
        }
        completedFiles += numberToDo;
    }*/

    system("pause");
    return 0;
}