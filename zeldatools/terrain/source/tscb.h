#pragma once
#include "TSCBTile.h"
#include <fstream>
#include <vector>
#include "TSCBHeader.h"
#include "TSCBMaterialTable.h"

class TSCB
{
public:
    TSCB();
    ~TSCB();

private:
    std::ifstream tscbFile;
    std::ofstream outputFile;

    void readHeader();
    void readMaterialTable();
    void readTiles();

    void writeFile();

    TSCBHeader headerInfo;
    TSCBMaterialTable materialTable[88];
    TSCBTile tiles[9033];
};