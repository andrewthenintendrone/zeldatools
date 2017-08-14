#include "TSCB.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <climits>
#include "endian.h"


TSCB::TSCB()
{
    m_tscbInputFile.open("MainField.tscb", std::ios::binary | std::ios::_Nocreate);

    if (!m_tscbInputFile.is_open())
    {
        std::cout << "Failed to open MainField.tscb\n";
    }

    m_tscbOutputFile.open("output.txt", std::ios::trunc);

    if (!m_tscbInputFile.is_open())
    {
        std::cout << "Failed to open MainField.tscb\n";
    }

    readHeader();
}

TSCB::~TSCB()
{
    
}

void TSCB::readHeader()
{
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.magic), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknown), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknown2), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.stringTableOffset), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknownFloat), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknownFloat2), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.materialLookupTableCount), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.tileTableCount), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknown3), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknown4), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknownFloat3), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.unknown5), 4);
    m_tscbInputFile.read(reinterpret_cast<char*>(&m_headerInfo.sizeOfMaterialLookupTable), 4);

    m_headerInfo.unknown2 = swap_endian<uint32_t>(m_headerInfo.unknown2);
    m_headerInfo.stringTableOffset = swap_endian<uint32_t>(m_headerInfo.stringTableOffset);
    m_headerInfo.unknownFloat = swap_endian<float>(m_headerInfo.unknownFloat);
    m_headerInfo.unknownFloat2 = swap_endian<float>(m_headerInfo.unknownFloat2);
    m_headerInfo.materialLookupTableCount = swap_endian<uint32_t>(m_headerInfo.materialLookupTableCount);
    m_headerInfo.tileTableCount = swap_endian<uint32_t>(m_headerInfo.tileTableCount);
    m_headerInfo.unknownFloat3 = swap_endian<float>(m_headerInfo.unknownFloat3);
    m_headerInfo.unknown5 = swap_endian<uint32_t>(m_headerInfo.unknown5);
    m_headerInfo.sizeOfMaterialLookupTable = swap_endian<uint32_t>(m_headerInfo.sizeOfMaterialLookupTable);

    readMaterialTable();
}

void TSCB::readMaterialTable()
{
    for (int i = 0; i < 88; i++)
    {
        // store file position
        uint32_t fileBookmark = m_tscbInputFile.tellg();

        // read current material offset
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].offset), 4);

        m_materials[i].offset = swap_endian<uint32_t>(m_materials[i].offset);
        
        // jump to material struct and read it
        m_tscbInputFile.seekg(m_materials[i].offset - 4, std::ios::cur);

        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].index), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].red), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].green), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].blue), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_materials[i].alpha), 4);

        m_materials[i].index = swap_endian<uint32_t>(m_materials[i].index);
        m_materials[i].red = swap_endian<float>(m_materials[i].red);
        m_materials[i].green = swap_endian<float>(m_materials[i].green);
        m_materials[i].blue = swap_endian<float>(m_materials[i].blue);
        m_materials[i].alpha = swap_endian<float>(m_materials[i].alpha);

        // return to the stored offset for next read
        m_tscbInputFile.seekg(fileBookmark + 4, std::ios::beg);
    }

    readTiles();
}

void TSCB::readTiles()
{
    m_tscbInputFile.seekg(0x874);

    for (int i = 0; i < 9033; i++)
    {
        // store file position
        uint32_t fileBookmark = m_tscbInputFile.tellg();

        // read current tile offset
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].offset), 4);

        m_tiles[i].offset = swap_endian<uint32_t>(m_tiles[i].offset);

        // jump to material struct and read it
        m_tscbInputFile.seekg(m_tiles[i].offset - 4, std::ios::cur);


        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].centerX), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].centerY), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].edgeLength), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk0), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk1), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk2), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk3), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk4), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].stringOffset), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk5), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk6), 4);
        m_tscbInputFile.read(reinterpret_cast<char*>(&m_tiles[i].unk7), 4);

        m_tiles[i].centerX = swap_endian<float>(m_tiles[i].centerX);
        m_tiles[i].centerY = swap_endian<float>(m_tiles[i].centerY);
        m_tiles[i].edgeLength = swap_endian<float>(m_tiles[i].edgeLength);
        m_tiles[i].unk0 = swap_endian<float>(m_tiles[i].unk0);
        m_tiles[i].unk1 = swap_endian<float>(m_tiles[i].unk1);
        m_tiles[i].unk2 = swap_endian<float>(m_tiles[i].unk2);
        m_tiles[i].unk3 = swap_endian<float>(m_tiles[i].unk3);
        m_tiles[i].unk4 = swap_endian<uint32_t>(m_tiles[i].unk4);
        m_tiles[i].stringOffset = swap_endian<uint32_t>(m_tiles[i].stringOffset);
        m_tiles[i].unk5 = swap_endian<uint32_t>(m_tiles[i].unk5);
        m_tiles[i].unk6 = swap_endian<uint32_t>(m_tiles[i].unk6);
        m_tiles[i].unk7 = swap_endian<uint32_t>(m_tiles[i].unk7);

        m_tscbInputFile.seekg(m_tiles[i].stringOffset - 16, std::ios::cur);
        m_tscbInputFile.read(m_tiles[i].tileName, 12);

        // return to the stored offset for next read
        m_tscbInputFile.seekg(fileBookmark + 4, std::ios::beg);
    }

    writeFile();
}

void TSCB::writeFile()
{
    // write header info
    //m_tscbOutputFile << "Magic ID: " << m_headerInfo.magic << std::endl;
    //m_tscbOutputFile << "Unknown: " << m_headerInfo.unknown << std::endl;
    //m_tscbOutputFile << "Unknown 2: " << m_headerInfo.unknown2 << std::endl;
    //m_tscbOutputFile << "String table offset: 0x" << std::hex << m_headerInfo.stringTableOffset << std::dec << std::endl;
    //m_tscbOutputFile << "Unknown Float: " << m_headerInfo.unknownFloat << std::endl;
    //m_tscbOutputFile << "Unknown Float 2: " << m_headerInfo.unknownFloat2 << std::endl;
    //m_tscbOutputFile << "Material Lookup table count: " << m_headerInfo.materialLookupTableCount << std::endl;
    //m_tscbOutputFile << "Tile table count: " << m_headerInfo.tileTableCount << std::endl;
    //m_tscbOutputFile << "Unknown 3: " << m_headerInfo.unknown3 << std::endl;
    //m_tscbOutputFile << "Unknown 4: " << m_headerInfo.unknown4 << std::endl;
    //m_tscbOutputFile << "Unknown Float 3: " << m_headerInfo.unknownFloat3 << std::endl;
    //m_tscbOutputFile << "Unknown 5: " << m_headerInfo.unknown5 << std::endl;
    //m_tscbOutputFile << "Size of Material Lookup Table: 0x" << std::hex << m_headerInfo.sizeOfMaterialLookupTable << std::dec << std::endl << std::endl;

    // write material table info
    //for (int i = 0; i < 88; i++)
    //{
    //    m_tscbOutputFile << "Material offset: 0x" << std::hex << m_materials[i].offset << std::dec << std::endl;
    //    m_tscbOutputFile << "Material index: " << m_materials[i].index << std::endl;
    //    m_tscbOutputFile << "Material red: " << m_materials[i].red << std::endl;
    //    m_tscbOutputFile << "Material green: " << m_materials[i].green << std::endl;
    //    m_tscbOutputFile << "Material blue: " << m_materials[i].blue << std::endl;
    //    m_tscbOutputFile << "Material alpha: " << m_materials[i].alpha << std::endl << std::endl;
    //}

    // write tile table info
    for (int i = 0; i < 9033; i++)
    {
        //m_tscbOutputFile << "Offset: 0x" << std::hex << m_tiles[i].offset << std::dec << std::endl;
        //m_tscbOutputFile << "Tile " << m_tiles[i].tileName << std::endl;
        m_tscbOutputFile << "Center X: " << m_tiles[i].centerX << std::endl;
        m_tscbOutputFile << "Center Y: " << m_tiles[i].centerY << std::endl;
        m_tscbOutputFile << "Edge Length: " << m_tiles[i].edgeLength << std::endl << std::endl;
        //m_tscbOutputFile << "unk0: " << m_tiles[i].unk0 << std::endl;
        //m_tscbOutputFile << "unk1: " << m_tiles[i].unk1 << std::endl;
        //m_tscbOutputFile << "unk2: " << m_tiles[i].unk2 << std::endl;
        //m_tscbOutputFile << "unk3: " << m_tiles[i].unk3 << std::endl;
        //m_tscbOutputFile << "unk4: " << m_tiles[i].unk4 << std::endl;
        //m_tscbOutputFile << "String Offset: 0x" << std::hex << m_tiles[i].stringOffset << std::dec << std::endl;
        //m_tscbOutputFile << "unk5: " << m_tiles[i].unk5 << std::endl;
        //m_tscbOutputFile << "unk6: " << m_tiles[i].unk6 << std::endl;
        //m_tscbOutputFile << "unk7: " << m_tiles[i].unk7 << std::endl << std::endl;
    }

    m_tscbInputFile.close();
    m_tscbOutputFile.close();
}
