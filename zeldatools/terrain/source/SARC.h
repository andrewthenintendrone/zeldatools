#pragma once
#include <fstream>
#include <cstdint>

class string;

class SARC
{
public:
    SARC(std::string& fileName = std::string("MainFiled.tscb"));
    ~SARC();

private:

    std::ifstream m_SARCinputFile;
    std::ofstream m_SARCoutputFiles[4];

    void readHeaderInfo();
    void readFile();
    void writeFile();

    uint16_t m_numFiles = 0;
    uint32_t m_dataOffset = 0;
    char m_fileNames[4][16]{ 0 };
    uint16_t m_data[4][256][256]{ 0 };
};