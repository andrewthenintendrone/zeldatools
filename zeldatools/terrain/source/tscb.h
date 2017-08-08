#pragma once
#include "Tile.h"
#include <vector>

class TSCB
{
public:
    TSCB();
    ~TSCB();

    Tile tiles[9033];
private:
    FILE* tscbFile;
    FILE* outputFile;

    void readData();
    void readStrings();
    void writeFile();
};