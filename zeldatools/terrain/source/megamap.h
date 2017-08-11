#pragma once
#include "Tile.h"
#include <vector>
#include <cstdint>

class megamap
{
    std::vector<Tile*> m_tileGrid;

    // constructor sets up grid
    megamap()
    {
        for (int x = 0; x < 158; x++)
        {
            for (int y = 0; y < 123; y++)
            {
                Tile* newTile = new Tile;
                newTile->centerX = x;
                newTile->centerY = y;
                m_tileGrid.push_back(newTile);
            }
        }
    }

    // destructor cleans up
    ~megamap()
    {
        while (!m_tileGrid.empty())
        {
            delete m_tileGrid.back();
            m_tileGrid.pop_back();
        }
    }
};