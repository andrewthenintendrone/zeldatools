#include "TSCB.h"
#include <iostream>
#include "endian.h"
#include <stb\stb_image_write.h>
#include <algorithm>
#include "Array2D.h"
#include "HGHT.h"

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

TSCB& TSCB::getInstance()
{
	static TSCB instance;
	return instance;
}

TSCB::TSCB()
{
	readFile();
	fixTileOffsets();
}

void TSCB::readFile()
{
	// open MainField.tscb
	m_inputFile.open(fs::current_path().string() + "\\MainField.tscb", std::ios::binary | std::ios::_Nocreate);

	// ensure it opened correctly
	if (!m_inputFile.is_open())
	{
		std::cout << "Failed to open MainField.tscb" << std::endl;
		return;
	}

	// read the different parts of the file
	readHeader();
	readMaterialTableOffsets();
	readMaterialTable();
	readTileTableOffsets();
	readTileTable();
	readTileNames();

	// close input file
	m_inputFile.close();
}

void TSCB::readHeader()
{
	// read raw values
    m_inputFile.read(reinterpret_cast<char*>(&m_header.magic), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unk), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unk2), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.stringTableOffset), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unkFloat), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unkFloat2), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.materialLookupTableCount), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.tileTableCount), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unk3), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unk4), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unkFloat3), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.unk5), 4);
    m_inputFile.read(reinterpret_cast<char*>(&m_header.materialLookupTableSize), 4);

	// swap endian on certain values
    m_header.unk2 = swap_endian<uint32_t>(m_header.unk2);
    m_header.stringTableOffset = swap_endian<uint32_t>(m_header.stringTableOffset);
    m_header.unkFloat = swap_endian<float>(m_header.unkFloat);
    m_header.unkFloat2 = swap_endian<float>(m_header.unkFloat2);
    m_header.materialLookupTableCount = swap_endian<uint32_t>(m_header.materialLookupTableCount);
    m_header.tileTableCount = swap_endian<uint32_t>(m_header.tileTableCount);
    m_header.unkFloat3 = swap_endian<float>(m_header.unkFloat3);
    m_header.unk5 = swap_endian<uint32_t>(m_header.unk5);
    m_header.materialLookupTableSize = swap_endian<uint32_t>(m_header.materialLookupTableSize);

	std::cout << "Finished reading header at 0x" << std::hex << m_inputFile.tellg() << std::endl;
}

void TSCB::readMaterialTableOffsets()
{
	for (unsigned int i = 0; i < m_header.materialLookupTableCount; i++)
	{
		uint32_t currentOffset;
		m_inputFile.read(reinterpret_cast<char*>(&currentOffset), sizeof(uint32_t));
		currentOffset = swap_endian<uint32_t>(currentOffset);
		m_materialTableOffsets.push_back(currentOffset + uint32_t(m_inputFile.tellg()) - 4);
	}

	std::cout << "Finished reading material table offsets at 0x" << std::hex << m_inputFile.tellg() << std::endl;
}

void TSCB::readMaterialTable()
{
    for (size_t i = 0; i < m_materialTableOffsets.size(); i++)
    {
		// jump to the correct offset
		m_inputFile.seekg(std::streampos(m_materialTableOffsets[i]), std::ios::beg);

        // read a material
		TSCBMaterialInfo material;

		m_inputFile.read(reinterpret_cast<char*>(&material.index), sizeof(uint32_t));
		m_inputFile.read(reinterpret_cast<char*>(&material.red), sizeof(float));
		m_inputFile.read(reinterpret_cast<char*>(&material.green), sizeof(float));
		m_inputFile.read(reinterpret_cast<char*>(&material.blue), sizeof(float));
		m_inputFile.read(reinterpret_cast<char*>(&material.alpha), sizeof(float));

		// swap endian for all values
		material.index = swap_endian<uint32_t>(material.index);
		material.red = swap_endian<float>(material.red);
		material.green = swap_endian<float>(material.green);
		material.blue = swap_endian<float>(material.blue);
		material.alpha = swap_endian<float>(material.alpha);

		// add to vector
		m_materialTable.push_back(material);
    }

	std::cout << "Finished reading material table at 0x" << std::hex << m_inputFile.tellg() << std::endl;
}

void TSCB::readTileTableOffsets()
{
	for (unsigned int i = 0; i < m_header.tileTableCount; i++)
	{
		uint32_t currentOffset;
		m_inputFile.read(reinterpret_cast<char*>(&currentOffset), sizeof(uint32_t));
		currentOffset = swap_endian<uint32_t>(currentOffset);
		m_tileTableOffsets.push_back(currentOffset + uint32_t(m_inputFile.tellg()) - 4);
	}

	std::cout << "Finished reading tile table offsets at 0x" << std::hex << m_inputFile.tellg() << std::endl;
}

void TSCB::readTileTable()
{
    for (size_t i = 0; i < m_tileTableOffsets.size(); i++)
    {
		// jump to the correct offset
		m_inputFile.seekg(std::streampos(m_tileTableOffsets[i]), std::ios::beg);

		// read a tile
		TSCBTileInfo tileInfo;

        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.centerX), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.centerY), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.edgeLength), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk0), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk1), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk2), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk3), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk4), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.stringOffset), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk5), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk6), 4);
        m_inputFile.read(reinterpret_cast<char*>(&tileInfo.unk7), 4);

		// swap endian for all values
        tileInfo.centerX = swap_endian<float>(tileInfo.centerX);
        tileInfo.centerY = swap_endian<float>(tileInfo.centerY);
        tileInfo.edgeLength = swap_endian<float>(tileInfo.edgeLength);
        tileInfo.unk0 = swap_endian<float>(tileInfo.unk0);
        tileInfo.unk1 = swap_endian<float>(tileInfo.unk1);
        tileInfo.unk2 = swap_endian<float>(tileInfo.unk2);
        tileInfo.unk3 = swap_endian<float>(tileInfo.unk3);
        tileInfo.unk4 = swap_endian<uint32_t>(tileInfo.unk4);
        tileInfo.stringOffset = swap_endian<uint32_t>(tileInfo.stringOffset);
		tileInfo.stringOffset += uint32_t(m_inputFile.tellg());
		tileInfo.stringOffset -= 16;
        tileInfo.unk5 = swap_endian<uint32_t>(tileInfo.unk5);
        tileInfo.unk6 = swap_endian<uint32_t>(tileInfo.unk6);
        tileInfo.unk7 = swap_endian<uint32_t>(tileInfo.unk7);

		// add to vector
		m_tileTable.push_back(tileInfo);
    }

	std::cout << "Finished reading tile table at 0x" << std::hex << m_inputFile.tellg() << std::endl;
}

void TSCB::readTileNames()
{
	// assume we are at the start of the string table
	for (size_t i = 0; i < m_tileTable.size(); i++)
	{
		m_inputFile.seekg(m_tileTable[i].stringOffset, std::ios::beg);

		std::string tileName;

		char c;

		while (true)
		{
			m_inputFile.read(&c, sizeof(char));

			// null terminator reached
			if (c == '\0')
			{
				break;
			}

			tileName += c;
		}

		m_tileNames.push_back(tileName);
	}

	std::cout << "Finished reading tile names at 0x" << std::hex << m_inputFile.tellg() << std::endl << std::endl;
}

void TSCB::writeFile()
{
	m_outputFile.open("output.txt", std::ios::trunc);

	if (!m_outputFile.is_open())
	{
		std::cout << "Failed to open output.txt\n";
	}

 //   // write header info
	//m_outputFile << "Magic ID: " << m_header.magic << std::endl;
 //   m_outputFile << "unk: " << m_header.unk << std::endl;
 //   m_outputFile << "unk 2: " << m_header.unk2 << std::endl;
 //   m_outputFile << "String table offset: 0x" << std::hex << m_header.stringTableOffset << std::dec << std::endl;
 //   m_outputFile << "unk Float: " << m_header.unkFloat << std::endl;
 //   m_outputFile << "unk Float 2: " << m_header.unkFloat2 << std::endl;
 //   m_outputFile << "Material Lookup table count: " << m_header.materialLookupTableCount << std::endl;
 //   m_outputFile << "Tile table count: " << m_header.tileTableCount << std::endl;
 //   m_outputFile << "unk 3: " << m_header.unk3 << std::endl;
 //   m_outputFile << "unk 4: " << m_header.unk4 << std::endl;
 //   m_outputFile << "unk Float 3: " << m_header.unkFloat3 << std::endl;
 //   m_outputFile << "unk 5: " << m_header.unk5 << std::endl;
 //   m_outputFile << "Size of Material Lookup Table: 0x" << std::hex << m_header.materialLookupTableSize << std::dec << std::endl << std::endl;

 //   // write material table info
 //   for (int i = 0; i < 88; i++)
 //   {
 //       m_outputFile << "Material offset: 0x" << std::hex << m_materialTableOffsets[i] << std::dec << std::endl;
 //       m_outputFile << "Material index: " << m_materialTable[i].index << std::endl;
 //       m_outputFile << "Material red: " << m_materialTable[i].red << std::endl;
 //       m_outputFile << "Material green: " << m_materialTable[i].green << std::endl;
 //       m_outputFile << "Material blue: " << m_materialTable[i].blue << std::endl;
 //       m_outputFile << "Material alpha: " << m_materialTable[i].alpha << std::endl << std::endl;
 //   }

    //// write tile table info
    //for (int i = 0; i < 9033; i++)
    //{
    //    //m_outputFile << "Offset: 0x" << std::hex << m_tileTableOffsets[i] << std::dec << std::endl;
    //    m_outputFile << "Tile " << m_tileNames[i] << std::endl;
    //    m_outputFile << "Center X: " << m_tileTable[i].centerX << std::endl;
    //    m_outputFile << "Center Y: " << m_tileTable[i].centerY << std::endl;
    //    m_outputFile << "Edge Length: " << m_tileTable[i].edgeLength << std::endl << std::endl;
    //    //m_outputFile << "unk0: " << m_tileTable[i].unk0 << std::endl;
    //    //m_outputFile << "unk1: " << m_tileTable[i].unk1 << std::endl;
    //    //m_outputFile << "unk2: " << m_tileTable[i].unk2 << std::endl;
    //    //m_outputFile << "unk3: " << m_tileTable[i].unk3 << std::endl;
    //    //m_outputFile << "unk4: " << m_tileTable[i].unk4 << std::endl;
    //    //m_outputFile << "String Offset: 0x" << std::hex << m_tileTable[i].stringOffset << std::dec << std::endl;
    //    //m_outputFile << "unk5: " << m_tileTable[i].unk5 << std::endl;
    //    //m_outputFile << "unk6: " << m_tileTable[i].unk6 << std::endl;
    //    //m_outputFile << "unk7: " << m_tileTable[i].unk7 << std::endl << std::endl;
    //}

	std::vector<std::string> filenames = getFileNamesByLOD(3);

	for (std::string filename : filenames)
	{
		m_outputFile << filename << std::endl;
	}

    m_outputFile.close();
}

// returns every filename
std::vector<std::string> TSCB::getAllFileNames()
{
	return m_tileNames;
}

bool sortByPosition(const std::string& a, const std::string& b)
{
	glm::vec2 posa = TSCB::getInstance().getTileLocation(a);
	glm::vec2 posb = TSCB::getInstance().getTileLocation(b);

	if (posa.y != posb.y)
	{
		return posa.y < posb.y;
	}

	return posa.x < posb.x;
}

// returns every file that is used in the specified LOD
std::vector<std::string> TSCB::getFileNamesByLOD(int LOD)
{
	char c = std::to_string(LOD)[0];
	std::vector<std::string> filenames;

	for (auto iter = m_tileNames.begin(); iter != m_tileNames.end(); iter++)
	{
		if ((*iter)[1] == c)
		{
			filenames.push_back(*iter);
		}
	}

	std::sort(filenames.begin(), filenames.end(), sortByPosition);

	return filenames;
}

TSCBTileInfo TSCB::getTileByName(const std::string& name)
{
	for (size_t i = 0; i < m_tileNames.size(); i++)
	{
		if (m_tileNames[i] == name)
		{
			return m_tileTable[i];
		}
	}

	return m_tileTable[0];
}

// get tile coordinates for tile with filename
glm::vec2 TSCB::getTileLocation(const std::string& filename)
{
	TSCBTileInfo tile = getTileByName(filename);

	return glm::vec2(tile.centerX, tile.centerY);
}

// write image containing the colors from the material table
void TSCB::writeColorImage(std::string& filename)
{
	// create and 88 x 1 image
	char pixelData[88][4];

	for (int x = 0; x < 88; x++)
	{
		pixelData[x][0] = (char)(m_materialTable[x].red * 255.0f);
		pixelData[x][1] = (char)(m_materialTable[x].green * 255.0f);
		pixelData[x][2] = (char)(m_materialTable[x].blue * 255.0f);
		pixelData[x][3] = (char)(m_materialTable[x].alpha * 255.0f);
	}

	stbi_write_png((filename + ".png").c_str(), 88, 1, 4, pixelData, 4);
}

// adjust tile centerX and centerY values to line up better with a grid
void TSCB::fixTileOffsets()
{
	int currentLOD = 0;

	for (size_t i = 0; i < m_tileTable.size(); i++)
	{
		if (m_tileNames[i].substr(1, 1) != std::to_string(currentLOD))
		{
			currentLOD++;
		}

		m_tileTable[i].centerX -= lowest[currentLOD].x;
		m_tileTable[i].centerX /= m_tileTable[i].edgeLength;
		m_tileTable[i].centerY -= lowest[currentLOD].y;
		m_tileTable[i].centerY /= m_tileTable[i].edgeLength;
	}
}
