#pragma once
#include <fstream>
#include <cstdint>
#include "HGHT.h"

class SSTERA
{

public:

    SSTERA(const std::string& filename);
	SSTERA(HGHT hghts[4]);
	void writeFiles();
    ~SSTERA();

private:

	std::string m_filename;
    std::ifstream m_inputFile;
    std::ofstream m_outputFiles[4];

    void readHeaderInfo();
    void readFile();

    uint16_t m_numFiles = 0;
    uint32_t m_dataOffset = 0;
    uint32_t m_dataSize = 0;
    char m_filenames[4][16]{ 0 };
    uint16_t m_data[4][256][256]{ 0 };
};