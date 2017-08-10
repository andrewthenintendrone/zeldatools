#pragma once
#include <fstream>
#include <cstdint>

class string;

class SARC
{
public:

    SARC(const std::string& fileName);
    ~SARC();

private:

    std::ifstream m_inputFile;
    std::ofstream m_outputFile;

    void readHeaderInfo();
    void readFiles();
    void writeFiles();

    uint16_t m_numFiles;
    uint32_t m_dataOffset;
    char m_fileNames[4][16];
    uint16_t m_data[4][65536];
};