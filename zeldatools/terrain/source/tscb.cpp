#include "tscb.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <climits>
#include "TSCBHeader.h"
#include "TSCBMaterialTable.h"

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
    tscbFile.open("MainField.tscb", std::ios::binary | std::ios::_Nocreate);

    if (!tscbFile.is_open())
    {
        std::cout << "Failed to open MainField.tscb\n";
    }

    outputFile.open("output.txt", std::ios::trunc);

    if (!tscbFile.is_open())
    {
        std::cout << "Failed to open MainField.tscb\n";
    }

    readHeader();
}

TSCB::~TSCB()
{
    tscbFile.close();
    outputFile.close();
}

void TSCB::readHeader()
{
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.magic), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknown), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknown2), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.stringTableOffset), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknownFloat), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknownFloat2), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.materialLookupTableCount), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.tileTableCount), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknown3), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknown4), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknownFloat3), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.unknown5), 4);
    tscbFile.read(reinterpret_cast<char*>(&headerInfo.sizeOfMaterialLookupTable), 4);

    headerInfo.unknown2 = swap_endian<uint32_t>(headerInfo.unknown2);
    headerInfo.stringTableOffset = swap_endian<uint32_t>(headerInfo.stringTableOffset);
    headerInfo.unknownFloat = swap_endian<float>(headerInfo.unknownFloat);
    headerInfo.unknownFloat2 = swap_endian<float>(headerInfo.unknownFloat2);
    headerInfo.materialLookupTableCount = swap_endian<uint32_t>(headerInfo.materialLookupTableCount);
    headerInfo.tileTableCount = swap_endian<uint32_t>(headerInfo.tileTableCount);
    headerInfo.unknownFloat3 = swap_endian<float>(headerInfo.unknownFloat3);
    headerInfo.unknown5 = swap_endian<uint32_t>(headerInfo.unknown5);
    headerInfo.sizeOfMaterialLookupTable = swap_endian<uint32_t>(headerInfo.sizeOfMaterialLookupTable);

    outputFile << tscbFile.tellg() << std::endl;
    readMaterialTable();
}

void TSCB::readMaterialTable()
{
    for (int i = 0; i < 88; i++)
    {
        // read current material offset
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].offset), 4);

        // store file position
        std::streampos fileBookmark = tscbFile.tellg();

        materialTable[i].offset = swap_endian<uint32_t>(materialTable[i].offset);
        
        // jump to material struct and read it
        tscbFile.seekg(materialTable[i].offset, std::ios::cur);

        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].index), 4);
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].red), 4);
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].green), 4);
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].blue), 4);
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].alpha), 4);

        materialTable[i].index = swap_endian<uint32_t>(materialTable[i].index);
        materialTable[i].red = swap_endian<float>(materialTable[i].red);
        materialTable[i].green = swap_endian<float>(materialTable[i].green);
        materialTable[i].blue = swap_endian<float>(materialTable[i].blue);
        materialTable[i].alpha = swap_endian<float>(materialTable[i].alpha);

        // return to the stored offset for next read
        tscbFile.seekg(fileBookmark, std::ios::beg);
    }

    outputFile << tscbFile.tellg() << std::endl;
    writeFile();
}

void TSCB::readStrings()
{
    //for (int i = 0; i < 9033; i++)
    //{
    //    // read name from string table
    //    fseek(tscbFile, tiles[i].stringOffset, SEEK_SET);
    //    std::cout << ftell(tscbFile) << std::endl;
    //    fread((void*)&tiles[i].tileName, sizeof(char), 11, tscbFile);
    //}

    //writeFile();
}

void TSCB::writeFile()
{
    outputFile << "Magic ID: " << headerInfo.magic << std::endl;
    outputFile << "Unknown: " << headerInfo.unknown << std::endl;
    outputFile << "Unknown 2: " << headerInfo.unknown2 << std::endl;
    outputFile << "String table offset: 0x" << std::hex << headerInfo.stringTableOffset << std::dec << std::endl;
    outputFile << "Unknown Float: " << headerInfo.unknownFloat << std::endl;
    outputFile << "Unknown Float 2: " << headerInfo.unknownFloat2 << std::endl;
    outputFile << "Material Lookup table count: " << headerInfo.materialLookupTableCount << std::endl;
    outputFile << "Tile table count: " << headerInfo.tileTableCount << std::endl;
    outputFile << "Unknown 3: " << headerInfo.unknown3 << std::endl;
    outputFile << "Unknown 4: " << headerInfo.unknown4 << std::endl;
    outputFile << "Unknown Float 3: " << headerInfo.unknownFloat3 << std::endl;
    outputFile << "Unknown 5: " << headerInfo.unknown5 << std::endl;
    outputFile << "Size of Material Lookup Table: " << headerInfo.sizeOfMaterialLookupTable << std::endl << std::endl;

    for (int i = 0; i < 88; i++)
    {
        outputFile << "Material offset: 0x" << std::hex << materialTable[i].offset << std::dec << std::endl;
        outputFile << "Material index: " << materialTable[i].index << std::endl;
        outputFile << "Material red: " << materialTable[i].red << std::endl;
        outputFile << "Material green: " << materialTable[i].green << std::endl;
        outputFile << "Material blue: " << materialTable[i].blue << std::endl;
        outputFile << "Material alpha: " << materialTable[i].alpha << std::endl << std::endl;
    }
}
