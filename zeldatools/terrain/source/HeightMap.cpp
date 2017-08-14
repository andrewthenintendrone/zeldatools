#include "HeightMap.h"
#include <algorithm>
#include "TSCB.h"
#include <iostream>
#include "endian.h"
#include <string>

std::string getProgramPath();

HeightMap::HeightMap(int qualityLevel)
{
    m_qualitySearchString = "5" + std::to_string(qualityLevel) + "000";

    // read, and write heights
    readFileNames();
    readHeights();
    sortHeights();
    writeHeights();
}

HeightMap::~HeightMap()
{

}

void HeightMap::readFileNames()
{
    std::ifstream unpackedFileNames;
    unpackedFileNames.open("unpackedFileNames.txt", std::ios::binary | std::ios::_Nocreate);

    // check if it opened
    if (!unpackedFileNames.is_open())
    {
        std::cout << "failed to open file list" << std::endl;
        system("pause");
    }

    while (!unpackedFileNames.eof())
    {
        std::string currentFileName;
        unpackedFileNames >> currentFileName;
        m_fileNames.push_back(currentFileName);
    }

    unpackedFileNames.close();
}

void HeightMap::readHeights()
{
    std::cout << "Reading Heights...\n";

    std::string readFolder = getProgramPath() + "\\output\\";

    for (unsigned int i = 0; i < m_fileNames.size(); i++)
    {
        if (m_fileNames[i].substr(0, 5) == m_qualitySearchString)
        {
            // open current input file
            m_HeightMapInputFile.open(readFolder + m_fileNames[i], std::ios::binary | std::ios::_Nocreate);

            Tile* currentTile = new Tile;

            // read the heightmap data
            for (int x = 255; x > -1; x--)
            {
                for (int y = 0; y < 256; y++)
                {
                    m_HeightMapInputFile.read(reinterpret_cast<char*>(&currentTile->m_heights[x][y]), sizeof(uint16_t));
                }
            }

            // set the x and y center using the tscb file
            currentTile->m_xOffset = m_tscbFile.m_tiles[i].centerX / m_tscbFile.m_tiles[i].edgeLength;
            currentTile->m_yOffset = m_tscbFile.m_tiles[i].centerY / m_tscbFile.m_tiles[i].edgeLength;

            m_tiles.push_back(currentTile);

            // close the current input file
            m_HeightMapInputFile.close();
        }
    }

    std::cout << "Read heights\n";
}

bool tileSort(Tile* a, Tile* b)
{
    if (a->m_yOffset < b->m_yOffset)
    {
        return true;
    }
    else if (a->m_yOffset > b->m_yOffset)
    {
        return false;
    }
    else
    {
        return (a->m_xOffset < b->m_xOffset);
    }
}

void HeightMap::sortHeights()
{
    std::sort(m_tiles.begin(), m_tiles.end(), tileSort);

    m_heightMapWidth = (m_tiles.back()->m_xOffset - m_tiles.front()->m_xOffset);
    m_heightMapHeight = (m_tiles.back()->m_yOffset - m_tiles.front()->m_yOffset);

    std::cout << m_tiles.size() << std::endl;

    for (int i = 0; i < m_tiles.size(); i++)
    {
        std::cout << "x: " << m_tiles[i]->m_xOffset << ", y: " << m_tiles[i]->m_yOffset << std::endl;
    }
}

void HeightMap::writeHeights()
{
    std::cout << "writing data...\n";

    // open output file
    m_HeightMapOutputFile.open("hyrule.raw", std::ios::binary | std::ios::trunc);

    // check if the file opened properly
    if (!m_HeightMapOutputFile.is_open())
    {
        std::cout << "failed to open hyrule.raw" << std::endl;
        system("pause");
    }

    std::cout << m_heightMapWidth * 256 << ", " << m_heightMapHeight * 256 << std::endl;

    for (int tileY = m_heightMapHeight; tileY > -1; tileY--)
    {
        for (int y = 0; y < 256; y++)
        {
            for (int tileX = 0; tileX < m_heightMapWidth; tileX++)
            {
                for (int x = 0; x < 256; x++)
                {
                    // m_HeightMapOutputFile << x + m_tiles[i]->m_xOffset * 256 << " " << y - m_tiles[i]->m_yOffset * 256 << " " << m_tiles[i]->m_heights[y][x] << "\n";
                    m_HeightMapOutputFile.write(reinterpret_cast<char*>(&m_tiles[tileY * m_heightMapWidth + tileX]->m_heights[y][x]), sizeof(uint16_t));
                    m_HeightMapOutputFile.write(reinterpret_cast<char*>(&m_tiles[tileY * m_heightMapWidth + tileX]->m_heights[y][x]), sizeof(uint16_t));
                    m_HeightMapOutputFile.write(reinterpret_cast<char*>(&m_tiles[tileY * m_heightMapWidth + tileX]->m_heights[y][x]), sizeof(uint16_t));
                }
            }
        }
    }

    std::cout << "wrote file\n";

    m_HeightMapOutputFile.close();
}
