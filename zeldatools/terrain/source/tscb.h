#pragma once
#include <fstream>
#include "TSCBHeader.h"
#include "TSCBMaterial.h"
#include "TSCBTile.h"

class TSCB
{
public:
    TSCB();
    ~TSCB();

    TSCBTile m_tiles[9033];
private:
    std::ifstream m_tscbInputFile;
    std::ofstream m_tscbOutputFile;

    void readHeader();
    void readMaterialTable();
    void readTiles();

    void writeFile();

    TSCBHeader m_headerInfo;
    TSCBMaterial m_materials[88];
};