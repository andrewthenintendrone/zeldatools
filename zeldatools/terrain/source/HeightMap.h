#pragma once
#include <Windows.h>
#include <string>
#include <fstream>

class HeightMap
{
public:
    HeightMap();
    HeightMap(std::string& newFileName, std::string& newShortFileName);
    ~HeightMap();

    void getHighestPoint();

    uint16_t heights[4][256][256] = { 0 };

private:
    std::string fileName;
    std::string shortFileName;
    std::FILE* ssteraFile = nullptr;
    std::FILE* rawFile = nullptr;
    void readChunkOrder();
    void readHeightMap();
    void writeHeightMap();
    uint8_t numChunks;
    char chunkOrder[4] = { 0 };
    int highestPoint = 0;
};