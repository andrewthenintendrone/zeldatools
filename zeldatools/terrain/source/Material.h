#pragma once
#include <Windows.h>
#include <fstream>

class Material
{
public:
    Material();
    Material(std::string& newFileName, std::string& newShortFileName);
    ~Material();

    RGBQUAD pixels[4][256][256] = { 0 };

private:
    std::string fileName;
    std::string shortFileName;
    std::FILE* ssteraFile = nullptr;
    std::FILE* imageFile = nullptr;
    void readChunkOrder();
    void writeHeaders();
    void readPixels();
    void writePixels();
    uint8_t numChunks;
    char chunkOrder[4] = { 0 };
};