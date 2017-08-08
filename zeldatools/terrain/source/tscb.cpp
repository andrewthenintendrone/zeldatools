#include "tscb.h"
#include <iostream>
#include <string>
#include <climits>

std::string getProgramPath();

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

TSCB::TSCB()
{
    // open tscb file
    tscbFile = fopen("MainField.tscb", "rb");

    // check if the file opened properly
    if (tscbFile == NULL)
    {
        std::cout << "failed to open MainField.tscb" << std::endl;
        system("pause");
        return;
    }

    // create output file
    outputFile = fopen("output.txt", "wb");

    // check if the file opened properly
    if (outputFile == NULL)
    {
        std::cout << "failed to create output.txt" << std::endl;
        system("pause");
        return;
    }

    readData();
}

TSCB::~TSCB()
{
    fclose(tscbFile);
    fclose(outputFile);
}

void TSCB::readData()
{
    // seek to the start of tile block
    fseek(tscbFile, 0x1A0, SEEK_SET);

    for (int i = 0; i < 9033; i++)
    {
        // read tile data from file
        fread((void*)&tiles[i], 48, 1, tscbFile);

        // convert from big to little endian
        tiles[i].centerX = swap_endian<float>(tiles[i].centerX);
        tiles[i].centerY = swap_endian<float>(tiles[i].centerY);
        tiles[i].edgeLength = swap_endian<float>(tiles[i].edgeLength);
        tiles[i].unk0 = swap_endian<float>(tiles[i].unk0);
        tiles[i].unk1 = swap_endian<float>(tiles[i].unk1);
        tiles[i].unk2 = swap_endian<float>(tiles[i].unk2);
        tiles[i].unk3 = swap_endian<float>(tiles[i].unk3);
        tiles[i].unk4 = swap_endian<uint32_t>(tiles[i].unk4);
        tiles[i].stringOffset = swap_endian<uint32_t>(tiles[i].stringOffset);
        tiles[i].unk5 = swap_endian<uint32_t>(tiles[i].unk5);
        tiles[i].unk6 = swap_endian<uint32_t>(tiles[i].unk6);
        tiles[i].unk7 = swap_endian<uint32_t>(tiles[i].unk7);

        if (tiles[i].unk7 != 0)
        {
            uint32_t extradata;
            fread(&extradata, sizeof(uint32_t), 1, tscbFile);
            extradata = swap_endian<uint32_t>(extradata);
            fseek(tscbFile, sizeof(uint32_t) * extradata, SEEK_CUR);
        }
    }

    //readStrings();
    writeFile();
}

void TSCB::readStrings()
{
    for (int i = 0; i < 9033; i++)
    {
        // read name from string table
        fseek(tscbFile, tiles[i].stringOffset, SEEK_SET);
        std::cout << ftell(tscbFile) << std::endl;
        fread((void*)&tiles[i].tileName, sizeof(char), 11, tscbFile);
    }

    writeFile();
}

void TSCB::writeFile()
{
    for (int i = 0; i < 9033; i++)
    {
        std::string result;
//        result += "tile name: " + std::string(tiles[i].tileName) + '\n';
        result += "center x: " + std::to_string(tiles[i].centerX) + '\n';
        result += "center y: " + std::to_string(tiles[i].centerY) + '\n';
        result += "edge length: " + std::to_string(tiles[i].edgeLength) + "\n\n";

        fputs(result.c_str(), outputFile);
    }
}