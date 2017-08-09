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

    readMaterialTable();
}

void TSCB::readMaterialTable()
{
    for (int i = 0; i < 88; i++)
    {
        // store file position
        uint32_t fileBookmark = tscbFile.tellg();

        // read current material offset
        tscbFile.read(reinterpret_cast<char*>(&materialTable[i].offset), 4);

        materialTable[i].offset = swap_endian<uint32_t>(materialTable[i].offset);
        
        // jump to material struct and read it
        tscbFile.seekg(materialTable[i].offset - 4, std::ios::cur);

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
        if (i != 87)
        {
            tscbFile.seekg(fileBookmark + 4, std::ios::beg);
        }
    }

    readTiles();
}

void TSCB::readTiles()
{
    outputFile << "about to read tiles from the address 0x" << std::hex << tscbFile.tellg() << std::endl;
    for (int i = 0; i < 9033; i++)
    {
        // read from file
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].centerX), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].centerY), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].edgeLength), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk0), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk1), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk2), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk3), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk4), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].stringOffset), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk5), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk6), 4);
        tscbFile.read(reinterpret_cast<char*>(&tiles[i].unk7), 4);

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
            tscbFile.read(reinterpret_cast<char*>(&tiles[i].extradata), 4);
            tiles[i].extradata = swap_endian<uint32_t>(tiles[i].extradata);
            outputFile << "Found a number that was not 0. The number was " << std::dec << tiles[i].extradata << " and it was found at 0x" << std::hex << tscbFile.tellg() << std::dec << std::endl;
        }
    }

    writeFile();
}

void TSCB::writeFile()
{
    // write header info
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
    outputFile << "Size of Material Lookup Table: 0x" << std::hex << headerInfo.sizeOfMaterialLookupTable << std::dec << std::endl << std::endl;

    // write material table info
    /*for (int i = 0; i < 88; i++)
    {
        outputFile << "Material offset: 0x" << std::hex << materialTable[i].offset << std::dec << std::endl;
        outputFile << "Material index: " << materialTable[i].index << std::endl;
        outputFile << "Material red: " << materialTable[i].red << std::endl;
        outputFile << "Material green: " << materialTable[i].green << std::endl;
        outputFile << "Material blue: " << materialTable[i].blue << std::endl;
        outputFile << "Material alpha: " << materialTable[i].alpha << std::endl << std::endl;
    }*/

    // write tile table info
    for (int i = 0; i < 9033; i++)
    {
        outputFile << "Center X: " << tiles[i].centerX << std::endl;
        outputFile << "Center Y: " << tiles[i].centerY << std::endl;
        outputFile << "Edge Length: " << tiles[i].edgeLength << std::endl;
        outputFile << "unk0: " << tiles[i].unk0 << std::endl;
        outputFile << "unk1: " << tiles[i].unk1 << std::endl;
        outputFile << "unk2: " << tiles[i].unk2 << std::endl;
        outputFile << "unk3: " << tiles[i].unk3 << std::endl;
        outputFile << "unk4: " << tiles[i].unk4 << std::endl;
        outputFile << "String Offset: 0x" << std::hex << tiles[i].stringOffset << std::dec << std::endl;
        outputFile << "unk5: " << tiles[i].unk5 << std::endl;
        outputFile << "unk6: " << tiles[i].unk6 << std::endl;
        outputFile << "unk7: " << tiles[i].unk7 << std::endl << std::endl;
    }
}
