#include "Material.h"
#include <string>
#include <windows.h>
#include <iostream>

std::string getProgramPath();

// per-texture constants
//unsigned int tileW = 4;
//unsigned int tileH = 4;
//unsigned int widthInTiles = (width + tileW - 1) / tileW;

Material::Material()
{

}

Material::Material(std::string& newFileName, std::string& newShortFileName)
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

    // create image file
    imageFile = fopen(std::string(getProgramPath() + "\\materials\\" + shortFileName + ".bmp").c_str(), "wb");

    // check if the file opened properly
    if (imageFile == NULL)
    {
        std::cout << "failed to create " << shortFileName << ".bmp" << std::endl;
        system("pause");
        return;
    }

    readChunkOrder();
}

Material::~Material()
{
    // close files
    fclose(ssteraFile);
    fclose(imageFile);
}

void Material::readChunkOrder()
{
    // read number of chunks
    fseek(ssteraFile, 0x1B, SEEK_SET);
    fread((void*)&numChunks, sizeof(uint8_t), 1, ssteraFile);
    std::cout << "found " << (int)numChunks << " chunk(s)\n";

    // move to the start of filename data
    fseek(ssteraFile, 0x28 + 0x10 * numChunks, SEEK_SET);

    char lowestChunk = 'Z';
    // read chunk order
    for (int i = 0; i < numChunks; i++)
    {
        // jump 9 characters
        fseek(ssteraFile, 9, SEEK_CUR);
        fread((void*)&chunkOrder[i], sizeof(char), 1, ssteraFile);
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

    writeHeaders();
}

void Material::writeHeaders()
{
    BITMAPINFOHEADER BMIH;                         // BMP header
    BMIH.biSize = sizeof(BITMAPINFOHEADER);
    BMIH.biSizeImage = 512 * 512 * 4;
    // Create the bitmap for this OpenGL context
    BMIH.biSize = sizeof(BITMAPINFOHEADER);
    BMIH.biWidth = 512;
    BMIH.biHeight = 512;
    BMIH.biPlanes = 1;
    BMIH.biBitCount = 32;
    BMIH.biCompression = BI_RGB;
    BMIH.biSizeImage = 512 * 512 * 4;

    BITMAPFILEHEADER bmfh;                         // Other BMP header
    int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize;
    LONG lImageSize = BMIH.biSizeImage;
    LONG lFileSize = nBitsOffset + lImageSize;
    bmfh.bfType = 'B' + ('M' << 8);
    bmfh.bfOffBits = nBitsOffset;
    bmfh.bfSize = lFileSize;
    bmfh.bfReserved1 = bmfh.bfReserved2 = 0;

    // Write the bitmap file header               // Saving the first header to file
    fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, imageFile);

    // And then the bitmap info header            // Saving the second header to file
    fwrite(&BMIH, sizeof(BITMAPINFOHEADER), 1, imageFile);

    readPixels();
}

void Material::readPixels()
{
    for (int i = 0; i < numChunks; i++)
    {
        fread((void*)&pixels[chunkOrder[i]], sizeof(pixels[chunkOrder[i]]), 1, ssteraFile);
    }

    writePixels();
}

void Material::writePixels()
{
    for (int y = 255; y > -1; y--)
    {
        fwrite(&pixels[2][y], sizeof(pixels[2][y]), 1, imageFile);
        fwrite(&pixels[3][y], sizeof(pixels[3][y]), 1, imageFile);
    }

    for (int y = 255; y > -1; y--)
    {
        fwrite(&pixels[0][y], sizeof(pixels[0][y]), 1, imageFile);
        fwrite(&pixels[1][y], sizeof(pixels[1][y]), 1, imageFile);
    }
}