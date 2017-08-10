#include "SARC.h"
#include <iostream>
#include <string>
#include "endian.h"

std::string getProgramPath();

SARC::SARC(const std::string& fileName)
{
    // open input file
    m_inputFile.open(fileName, std::ios::binary | std::ios::_Nocreate);

    // check if file was opened correctly
    if (!m_inputFile.is_open())
    {
        std::cout << "Failed to open " << fileName << std::endl;
    }

    readHeaderInfo();
}

SARC::~SARC()
{
    m_inputFile.close();
}

// obtains the following info from the SARC header:
// number of files
// offset to file data
// file names
void SARC::readHeaderInfo()
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
    for (int j = 0; j < m_numFiles; j++)
    {
        // jump to start of filename table + current file name
        m_inputFile.seekg(0x28 + 0x10 * (m_numFiles + j), std::ios::beg);
        m_inputFile.read(m_fileNames[j], 0x10);
    }

    readFiles();
}

// reads each file into memory
void SARC::readFiles()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        // jump to current file offset
        m_inputFile.seekg(m_dataOffset + sizeof(m_data) * i, std::ios::beg);

        // read current file
        m_inputFile.read(reinterpret_cast<char*>(&m_data[i]), sizeof(m_data[i]));
    }

    writeFiles();
}

// wries files
void SARC::writeFiles()
{
    for (int i = 0; i < m_numFiles; i++)
    {
        m_outputFile.open((getProgramPath() + "\\output\\" + m_fileNames[i]), std::ios::binary | std::ios::trunc);

        if (!m_outputFile.is_open())
        {
            std::cout << "failed to open " << m_fileNames[i] << std::endl;
        }

        m_outputFile.write(reinterpret_cast<char*>(m_data[i]), sizeof(m_data[i]));

        m_outputFile.close();
    }
}
