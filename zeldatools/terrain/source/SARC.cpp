#include "SARC.h"
#include <iostream>
#include <string>
#include "endian.h"

std::string getProgramPath();

SARC::SARC(std::string& fileName)
{
    // open input file
    m_SARCinputFile.open(fileName, std::ios::binary | std::ios::_Nocreate);

    // check if file was opened correctly
    if (!m_SARCinputFile.is_open())
    {
        std::cout << "Failed to open " << fileName << std::endl;
        system("pause");
    }

    readHeaderInfo();
}

SARC::~SARC()
{

}

// obtains the following info from the SARC header:
// number of files
// offset to file data
// file names
void SARC::readHeaderInfo()
{
    // read data offset
    m_SARCinputFile.seekg(0xc, std::ios::beg);
    m_SARCinputFile.read(reinterpret_cast<char*>(&m_dataOffset), 4);
    m_dataOffset = swap_endian<uint32_t>(m_dataOffset);

    // read number of files
    m_SARCinputFile.seekg(0x1A, std::ios::beg);
    m_SARCinputFile.read(reinterpret_cast<char*>(&m_numFiles), 2);
    m_numFiles = swap_endian<uint16_t>(m_numFiles);

    // read file names
    for (int i = 0; i < m_numFiles; i++)
    {
        // jump to start of filename table + current file name
        m_SARCinputFile.seekg(0x28 + 0x10 * (m_numFiles + i), std::ios::beg);
        m_SARCinputFile.read(reinterpret_cast<char*>(&m_fileNames[i]), 0x10);
    }

    readFile();
}

// reads each file into memory
void SARC::readFile()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        // jump to current file offset
        m_SARCinputFile.seekg(m_dataOffset + (0x20000 * i), std::ios::beg);

        // read current file
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                m_SARCinputFile.read(reinterpret_cast<char*>(&m_data[i][x][y]), sizeof(uint16_t));
            }
        }
    }

    writeFile();
}

// wries files
void SARC::writeFile()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        m_SARCoutputFiles[i].open((getProgramPath() + "\\output\\" + m_fileNames[i]), std::ios::binary | std::ios::trunc);

        if (!m_SARCoutputFiles[i].is_open())
        {
            std::cout << "failed to open " << m_fileNames[i] << std::endl;
            system("pause");
        }

        // write current file
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                m_SARCoutputFiles[i].write(reinterpret_cast<char*>(&m_data[i][x][y]), sizeof(uint16_t));
            }
        }

        m_SARCoutputFiles[i].close();
    }

    m_SARCinputFile.close();
}
