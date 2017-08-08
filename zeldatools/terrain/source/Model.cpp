#include "Model.h"
#include <iostream>
#include <intrin.h>

char charToValue(char input);
std::string getProgramPath();

const int xOffsets[4] = { 0, 0, 256, 256 };
const int yOffsets[4] = { 0, 256, 0, 256 };

const int modelXZScale = 1;

Model::Model()
{

}

Model::Model(std::string& newFileName, std::string& newShortFileName)
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
    modelFile = fopen(std::string(getProgramPath() + "\\models\\" + shortFileName + ".obj").c_str(), "wb");

    // check if the file opened properly
    if (modelFile == NULL)
    {
        std::cout << "failed to create " << shortFileName << ".obj" << std::endl;
        system("pause");
        return;
    }

    readChunkOrder();
}

Model::~Model()
{
    // close files
    fclose(ssteraFile);
    fclose(modelFile);
}

void Model::readChunkOrder()
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

    writeHeader();
}

void Model::writeHeader()
{
    // write object header
    fputs(std::string("o " + shortFileName + "\n\n").c_str(), modelFile);

    readVerts();
}

void Model::readVerts()
{
    for (int i = 0; i < numChunks; i++)
    {
        fread((void*)&heights[chunkOrder[i]], sizeof(heights[chunkOrder[i]]), 1, ssteraFile);
    }

    writeVerts();
}

void Model::writeVerts()
{
    for (int i = 0; i < numChunks; i++)
    {
        for (int y = 255; y > -1; y--)
        {
            for (int x = 255; x > -1; x--)
            {
                fputs(std::string("v " + std::to_string((x + xOffsets[chunkOrder[i]]) * modelXZScale) + ".0 " + std::to_string(heights[chunkOrder[i]][x][y]) + ".0 " + std::to_string((y + yOffsets[chunkOrder[i]]) * modelXZScale) + ".0\n").c_str(), modelFile);
            }
        }
    }

    writeFaces();
}

void Model::writeFaces()
{
    for (int i = 0; i < numChunks; i++)
    {
        for (int v = 1; v < 65281; v++)
        {
            if (v % 256 != 0)
            {
                fputs(std::string("f " + std::to_string(i * 65536 + v) + " " + std::to_string(i * 65536 + v + 1) + " " + std::to_string(i * 65536 + v + 257) + " " + std::to_string(i * 65536 + v + 256) + "\n").c_str(), modelFile);
            }
        }
    }
}