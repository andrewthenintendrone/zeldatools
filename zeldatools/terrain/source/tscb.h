#pragma once
#include "Tile.h"
#include <fstream>
#include <vector>
#include "TSCBHeader.h"
#include "TSCBMaterialTable.h"

class TSCB
{
public:
    TSCB();
    ~TSCB();

    Tile tiles[9033];
private:
    std::ifstream tscbFile;
    std::ofstream outputFile;

    void readHeader();
    void readMaterialTable();
    void readStrings();
    void writeFile();

    TSCBHeader headerInfo;
    TSCBMaterialTable materialTable[88];
};