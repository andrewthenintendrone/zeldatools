#pragma once
#include <Windows.h>
#include <string>
#include <fstream>

class Model
{
public:
    Model();
    Model(std::string& newFileName, std::string& newShortFileName);
    ~Model();

    uint16_t heights[4][256][256] = { 0 };

private:
    std::string fileName;
    std::string shortFileName;
    std::FILE* ssteraFile = nullptr;
    std::FILE* modelFile = nullptr;
    void readChunkOrder();
    void writeHeader();
    void readVerts();
    void writeVerts();
    void writeFaces();
    uint8_t numChunks;
    char chunkOrder[4] = { 0 };
};