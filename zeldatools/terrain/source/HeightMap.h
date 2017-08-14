#pragma once
#include "Tile.h"
#include "TSCB.h"
#include <vector>

class HeightMap
{
public:
    HeightMap(int qualityLevel);
    ~HeightMap();

private:

    const float qualityLevels[9]{ 32.0f, 16.0f, 8.0f, 4.0f, 2.0f, 1.0f, 0.5f, 0.25f, 0.125f };

    std::string m_qualitySearchString;
    std::vector<std::string> m_fileNames;

    TSCB m_tscbFile;

    int m_heightMapWidth;
    int m_heightMapHeight;

    std::ifstream m_HeightMapInputFile;
    std::ofstream m_HeightMapOutputFile;

    void readFileNames();
    void readHeights();
    void sortHeights();
    void writeHeights();

    std::vector<Tile*> m_tiles;
};