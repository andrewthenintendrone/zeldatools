#include "SSTERA.h"
#include <iostream>
#include <string>
#include "endian.h"
#include <Windows.h>

SSTERA::SSTERA()
{

}

// processes the file
void SSTERA::unpackSSTERA(const std::string& fileName)
{
    // open input file
    m_inputFile.open(fileName, std::ios::binary | std::ios::_Nocreate);

    // check if file was opened correctly
    if (!m_inputFile.is_open())
    {
        std::cout << "Failed to open " << fileName << std::endl;
        system("pause");
    }

    readHeaderInfo();
}

SSTERA::~SSTERA()
{

}

// obtains the following info from the SARC header:
// number of files
// offset to file data
// file names
void SSTERA::readHeaderInfo()
{
    // read data offset
    m_inputFile.seekg(0xc, std::ios::beg);
    m_inputFile.read(reinterpret_cast<char*>(&m_dataOffset), 4);
    m_dataOffset = swap_endian<uint32_t>(m_dataOffset);

    // read number of files
    m_inputFile.seekg(0x1A, std::ios::beg);
    m_inputFile.read(reinterpret_cast<char*>(&m_numFiles), 2);
    m_numFiles = swap_endian<uint16_t>(m_numFiles);

    // read file names
    for (int i = 0; i < m_numFiles; i++)
    {
        // jump to start of filename table + current file name
        m_inputFile.seekg(0x28 + 0x10 * (m_numFiles + i), std::ios::beg);
        m_inputFile.read(reinterpret_cast<char*>(&m_fileNames[i]), 0x10);
    }

    readFile();
}

// reads each file into memory
void SSTERA::readFile()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        // jump to current file offset
        m_inputFile.seekg(m_dataOffset + (0x20000 * i), std::ios::beg);

        // read current file
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                m_inputFile.read(reinterpret_cast<char*>(&m_data[i][x][y]), sizeof(uint16_t));
            }
        }
    }

    writeFile();
}

// wries files
void SSTERA::writeFile()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        //m_outputFiles[i].open(getProgramPath() + "\\" + m_fileNames[i], std::ios::binary | std::ios::trunc);

        if (!m_outputFiles[i].is_open())
        {
            std::cout << "failed to open " << m_fileNames[i] << std::endl;
            system("pause");
        }

        // write current file
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                m_outputFiles[i].write(reinterpret_cast<char*>(&m_data[i][x][y]), sizeof(uint16_t));
            }
        }

        m_outputFiles[i].close();
    }

    m_inputFile.close();
}
