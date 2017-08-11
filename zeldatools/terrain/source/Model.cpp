#include "Model.h"
#include <iostream>
#include "endian.h"
#include "fileNames.h"
#include <algorithm>

std::string getProgramPath();

Model::Model()
{
    // read, sort, and write heights
    readHeights();
    arrangeHeights();
    writeHeights();
}

Model::~Model()
{

}

void Model::readHeights()
{
    // go through each unpacked file
    for (unsigned int i = 0; i < fileNames.size(); i++)
    {
        // display progress
        system("cls");
        std::cout << "Reading Heights: " << i + 1 << "/" << fileNames.size() << std::endl;

        // open current input file
        m_ModelInputFiles[i].open(fileNames[i], std::ios::binary | std::ios::_Nocreate);

        // check if it opened
        //if (!m_ModelInputFiles[i].is_open())
        //{
        //    std::cout << "failed to open " << fileNames[i] << std::endl;
        //    system("pause");
        //}

        // create a new tile
        Tile* currentTile = new Tile;

        // read the heightmap data
        m_ModelInputFiles[i].read(reinterpret_cast<char*>(&currentTile->m_heights), sizeof(currentTile->m_heights));
        // calculate and set the x and y center
        currentTile->centerX = m_tscbFile.m_tiles[5291 + i].centerX / 0.125f + 78.5f;
        currentTile->centerY = m_tscbFile.m_tiles[5291 + i].centerY / 0.125f + 61.5f;

        // add the current tile to the list
        m_megamap

        // close the current input file
        m_ModelInputFiles[i].close();
    }
}

// sort tiles using a custom function object
struct
{
    bool operator()(Tile* a, Tile* b) const
    {
        if (a->centerY < b->centerY)
        {
            return true;
        }
        else if (a->centerY > b->centerY)
        {
            return false;
        }
        // same row
        else
        {
            return (a->centerX < b->centerX);
        }
    }
} tileSort;

void Model::arrangeHeights()
{
    std::cout << "arranging heights\n";
    std::sort(m_tiles.begin(), m_tiles.end(), tileSort);
}

void Model::writeHeights()
{
    std::cout << "writing data\n";

    // open output file
    m_ModelOutputFile.open("hyrule.raw", std::ios::binary | std::ios::trunc);

    // check if the file opened properly
    if (!m_ModelOutputFile.is_open())
    {
        std::cout << "failed to open hyrule.raw" << std::endl;
        system("pause");
    }

    
}
