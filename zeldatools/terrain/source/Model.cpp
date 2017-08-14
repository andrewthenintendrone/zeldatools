#include "Model.h"
#include <algorithm>
#include "TSCB.h"
#include <iostream>
#include "endian.h"
#include <string>

std::string getProgramPath();

Model::Model(int qualityLevel)
{
    m_qualitySearchString = "5" + std::to_string(qualityLevel) + "000";

    // read, and write heights
    readFileNames();
    readHeights();
    writeHeights();
}

Model::~Model()
{
}

void Model::readFileNames()
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

void Model::readHeights()
{
    std::cout << "Reading Heights...\n";

    std::string readFolder = getProgramPath() + "\\output\\";

    for (unsigned int i = 0; i < m_fileNames.size(); i++)
    {
        if (m_fileNames[i].substr(0, 5) == m_qualitySearchString)
        {
            // open current input file
            m_HeightMapInputFile.open(readFolder + m_fileNames[i], std::ios::binary | std::ios::_Nocreate);

            // check if it opened
            //if (!m_ModelInputFile.is_open())
            //{
            //    std::cout << "failed to open " << fileNames[i] << std::endl;
            //    system("pause");
            //}

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

// sort tiles using a custom function object
//struct
//{
//    bool operator()(Tile* a, Tile* b) const
//    {
//        if (a->centerY < b->centerY)
//        {
//            return true;
//        }
//        else if (a->centerY > b->centerY)
//        {
//            return false;
//        }
//        // same row
//        else
//        {
//            return (a->centerX < b->centerX);
//        }
//    }
//} tileSort;
//
//void Model::arrangeHeights()
//{
//    std::cout << "arranging heights\n";
//    std::sort(m_tiles.begin(), m_tiles.end(), tileSort);
//}

void Model::writeHeights()
{
    std::cout << "writing data...\n";

    // open output file
    m_ModelOutputFile.open("hyrule.ply", std::ios::binary | std::ios::trunc);

    // check if the file opened properly
    if (!m_ModelOutputFile.is_open())
    {
        std::cout << "failed to open hyrule.ply" << std::endl;
        system("pause");
    }

    // write ply header an info
    m_ModelOutputFile << "ply\n";
    m_ModelOutputFile << "format ascii 1.0\n";
    m_ModelOutputFile << "element vertex " << m_tiles.size() * 256 * 256 << "\n";
    m_ModelOutputFile << "property float x\n";
    m_ModelOutputFile << "property float y\n";
    m_ModelOutputFile << "property float z\n";
    m_ModelOutputFile << "end_header\n";

    for (unsigned int i = 0; i < m_tiles.size(); i++)
    {
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                m_ModelOutputFile << x + m_tiles[i]->m_xOffset * 256 << " " << y - m_tiles[i]->m_yOffset * 256 << " " << m_tiles[i]->m_heights[y][x] << "\n";
            }
        }
    }


    std::cout << "wrote file\n";

    m_ModelOutputFile.close();
}
