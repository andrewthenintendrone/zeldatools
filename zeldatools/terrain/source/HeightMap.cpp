#include "HeightMap.h"
#include <iostream>
#include <intrin.h>

char charToValue(char input);
std::string getProgramPath();

const int xOffsets[4] = { 0, 0, 256, 256 };
const int yOffsets[4] = { 0, 256, 0, 256 };

const int modelXZScale = 1;

HeightMap::HeightMap()
{

}

HeightMap::HeightMap(std::string& newFileName, std::string& newShortFileName)
{
    fileName = newFileName;
    shortFileName = newShortFileName;

    // open sstera file
    ssteraFile = fopen(fileName.c_str(), "rb");

    // check if the file opened properly
    if (ssteraFile == NULL)
    {
        std::cout << "failed to open " << shortFileName << ".sstera" << std::endl;
        system("pause");
        return;
    }

    // create model file
    rawFile = fopen(std::string(getProgramPath() + "\\models\\" + shortFileName + ".raw").c_str(), "wb");

    // check if the file opened properly
    if (rawFile == NULL)
    {
        std::cout << "failed to create " << shortFileName << ".raw" << std::endl;
        system("pause");
        return;
    }

    readChunkOrder();
}

HeightMap::~HeightMap()
{
    // close files
    fclose(ssteraFile);
    fclose(rawFile);
}

void HeightMap::readChunkOrder()
{
    // read number of chunks
    fseek(ssteraFile, 0x1B, SEEK_SET);
    fread((void*)&numChunks, sizeof(uint8_t), 1, ssteraFile);
    //std::cout << "found " << (int)numChunks << " chunks\n ";

    // move to the start of filename data
    fseek(ssteraFile, 0x28 + 0x10 * numChunks, SEEK_SET);

    char lowestChunk = 'Z';
    // read chunk order
    for (int i = 0; i < numChunks; i++)
    {
        // jump 9 characters
        fseek(ssteraFile, 9, SEEK_CUR);
        fread((void*)&chunkOrder[i], sizeof(char), 1, ssteraFile);
        chunkOrder[i] = charToValue(chunkOrder[i]);
        if (chunkOrder[i] < lowestChunk)
        {
            lowestChunk = chunkOrder[i];
        }
        // jump 6 characters
        fseek(ssteraFile, 6, SEEK_CUR);
    }

    std::cout << "the order of the chunk(s) is ";
    // align chunk order
    for (int i = 0; i < numChunks; i++)
    {
        chunkOrder[i] -= lowestChunk;
        std::cout << (int)chunkOrder[i] << " ";
    }
    std::cout << std::endl;

    readHeightMap();
}

void HeightMap::readHeightMap()
{
    for (int i = 0; i < numChunks; i++)
    {
        fread((void*)&heights[chunkOrder[i]], sizeof(heights[chunkOrder[i]]), 1, ssteraFile);
    }

    writeHeightMap();
}

void HeightMap::writeHeightMap()
{
    for (int y = 255; y > -1; y--)
    {
        fwrite(&heights[2][y], sizeof(heights[2][y]), 1, rawFile);
        fwrite(&heights[3][y], sizeof(heights[3][y]), 1, rawFile);
    }

    for (int y = 255; y > -1; y--)
    {
        fwrite(&heights[0][y], sizeof(heights[0][y]), 1, rawFile);
        fwrite(&heights[1][y], sizeof(heights[1][y]), 1, rawFile);
    }
}

void HeightMap::getHighestPoint()
{
    for (int i = 0; i < 4; i++)
    {
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                if (heights[i][x][y] > highestPoint)
                {
                    highestPoint = heights[i][x][y];
                }
            }
        }
    }

    std::cout << "The highest point on the terrain is " << highestPoint << std::endl;
}