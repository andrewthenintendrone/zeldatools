#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include "Model.h"
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
}

int main(int argc, char* argv[])
{
    Model model;
    //TSCB tscb;
    
    system("pause");
    return 0;
}