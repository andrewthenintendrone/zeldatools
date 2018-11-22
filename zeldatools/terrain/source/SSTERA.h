#pragma once
#include <fstream>
#include <cstdint>

class SSTERA
{
public:
    SSTERA();
    void unpackSSTERA(const std::string& fileName);
    ~SSTERA();

private:

    std::ifstream m_inputFile;
    std::ofstream m_outputFiles[4];

    void readHeaderInfo();
    void readFile();
    void writeFile();

    uint16_t m_numFiles = 0;
    uint32_t m_dataOffset = 0;
    uint32_t m_dataSize = 0;
    char m_fileNames[4][16]{ 0 };
    uint16_t m_data[4][256][256]{ 0 };
};