#include <Windows.h>
#include <iostream>
#include <string>
#include "SARC.h"
#include "TSCB.h"
#include "packedFiles.h"
#include <thread>

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

    SARC sarc(fileName);
}

int main(int argc, char* argv[])
{
    //TSCB tscb;
    //std::thread threads[maximumThreads];
    //int completedFiles = 1;

    //while (completedFiles < numFiles)
    //{

        //unsigned int numberToDo = numFiles - completedFiles;

        //if (numberToDo > maximumThreads)
        //{
        //    numberToDo = maximumThreads;
        //}

        for (unsigned int i = 0; i < numFiles; i++)
        {
            SARC sarc(fileNames[i]);
            system("cls");
            std::cout << "Finished: " << i << "/" << numFiles << std::endl;
            //threads[i] = std::thread(processFile, std::string(fileNames[completedFiles + i]));
        }
        //for (unsigned int i = 0; i < numberToDo; i++)
        //{
        //    threads[i].join();
        //}
        //completedFiles += numberToDo;
    //}

    system("pause");
    return 0;
}