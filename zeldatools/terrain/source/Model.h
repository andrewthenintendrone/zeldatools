#pragma once
#include <fstream>
#include "TSCB.h"
#include <vector>
#include "megamap.h"

class Model
{
public:
    Model();
    ~Model();

private:

    TSCB m_tscbFile;

    std::ifstream m_ModelInputFiles[3742];
    std::ofstream m_ModelOutputFile;

    void readHeights();
    void arrangeHeights();
    void writeHeights();

    megamap m_megamap;
};