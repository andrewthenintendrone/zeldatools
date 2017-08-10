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

private:
    std::ifstream m_inputFile;
    std::ofstream m_outputFile;

    void readHeader();
    void readMaterialTable();
    void readTiles();

    void writeFile();

    TSCBHeader m_headerInfo;
    TSCBMaterial m_materials[88];
    TSCBTile m_tiles[9033];
};