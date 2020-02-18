#include "TSCB.h"
#include <iostream>
#include <stb\stb_image_write.h>
#include <algorithm>
#include "Array2D.h"
#include "HGHT.h"
#include "filenameUtils.h"

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
	fixAreaOffsets();
}

void TSCB::readFile()
{
	// open MainField.tscb
	m_reader.open(fs::current_path().string() + "\\MainField.tscb");

	// read the different parts of the file
	readHeader();
	readMaterialTableOffsets();
	readMaterialTable();
	readAreaTableOffsets();
	readAreaTable();
	readAreaNames();
}

void TSCB::readHeader()
{
	// read raw values
	m_header.magic[0] = m_reader.read<char>();
	m_header.magic[1] = m_reader.read<char>();
	m_header.magic[2] = m_reader.read<char>();
	m_header.magic[3] = m_reader.read<char>();
	m_header.unk = m_reader.read<uint32_t>();
	m_header.unk2 = m_reader.read<uint32_t>(true);
	m_header.stringTableOffset = m_reader.read<uint32_t>(true);
	m_header.worldScale = m_reader.read<float>(true);
	m_header.terrainMeshMaxHeight = m_reader.read<float>(true);
	m_header.materialInfoArrayLength = m_reader.read<uint32_t>(true);
	m_header.areaArrayLength = m_reader.read<uint32_t>(true);
	m_header.padding = m_reader.read<uint32_t>();
	m_header.padding2 = m_reader.read<uint32_t>();
	m_header.tileSize = m_reader.read<float>(true);
	m_header.unk3 = m_reader.read<uint32_t>(true);
	m_header.materialLookupTableSize = m_reader.read<uint32_t>(true);

	std::cout << "Finished reading header at 0x" << std::hex << m_reader.getFileOffset() << std::endl;
}

void TSCB::readMaterialTableOffsets()
{
	for (unsigned int i = 0; i < m_header.materialInfoArrayLength; i++)
	{
		uint32_t currentOffset = m_reader.read<uint32_t>(true) + (uint32_t)m_reader.getFileOffset() - 4;
		m_materialTableOffsets.push_back(currentOffset);
	}

	std::cout << "Finished reading material table offsets at 0x" << std::hex << m_reader.getFileOffset() << std::endl;
}

void TSCB::readMaterialTable()
{
    for (size_t i = 0; i < m_materialTableOffsets.size(); i++)
    {
		// jump to the correct offset
		m_reader.seek(std::streampos(m_materialTableOffsets[i]));

        // read a material
		TSCBMaterialInfo material;

		material.index = m_reader.read<uint32_t>(true);
		material.textureUAxis = m_reader.read<float>(true);
		material.textureVAxis = m_reader.read<float>(true);
		material.unk = m_reader.read<float>(true);
		material.unk2 = m_reader.read<float>(true);

		// add to vector
		m_materialTable.push_back(material);
    }

	std::cout << "Finished reading material table at 0x" << std::hex << m_reader.getFileOffset() << std::endl;
}

void TSCB::readAreaTableOffsets()
{
	for (unsigned int i = 0; i < m_header.areaArrayLength; i++)
	{
		uint32_t currentOffset = m_reader.read<uint32_t>(true) + (uint32_t)m_reader.getFileOffset() - 4;
		m_areaTableOffsets.push_back(currentOffset);
	}

	std::cout << "Finished reading area table offsets at 0x" << std::hex << m_reader.getFileOffset() << std::endl;
}

void TSCB::readAreaTable()
{
    for (size_t i = 0; i < m_areaTableOffsets.size(); i++)
    {
		// jump to the correct offset
		m_reader.seek(std::streampos(m_areaTableOffsets[i]));

		// read an area
		TSCBArea area;

        area.xPosition = m_reader.read<float>(true);
        area.zPosition = m_reader.read<float>(true);
        area.areaSize = m_reader.read<float>(true);
        area.grassDensity = m_reader.read<float>(true);
		area.unk = m_reader.read<float>(true);
		area.unk2 = m_reader.read<float>(true);
		area.unk3 = m_reader.read<float>(true);
		area.unk4 = m_reader.read<uint32_t>(true);
		area.nameOffset = m_reader.read<uint32_t>(true);
		area.unk5 = m_reader.read<uint32_t>(true);
		area.unk6 = m_reader.read<uint32_t>(true);
		area.unk7 = m_reader.read<uint32_t>(true);

		// swap endian for all values
		area.nameOffset += (uint32_t)m_reader.getFileOffset() - 16;

		// add to vector
		m_areaTable.push_back(area);
    }

	std::cout << "Finished reading area table at 0x" << std::hex << m_reader.getFileOffset() << std::endl;
}

void TSCB::readAreaNames()
{
	// assume we are at the start of the string table
	for (size_t i = 0; i < m_areaTable.size(); i++)
	{
		m_reader.seek(m_areaTable[i].nameOffset);

		std::string areaName = m_reader.readNullTerminatedString();

		m_areaNames.push_back(areaName);
	}

	std::cout << "Finished reading area names at 0x" << std::hex << m_reader.getFileOffset() << std::endl << std::endl;
}

void TSCB::writeFile()
{
	std::ofstream outputFile;

	outputFile.open("output_DLC.txt", std::ios::trunc);

	if (!outputFile.is_open())
	{
		std::cout << "Failed to open output.txt\n";
	}

    // write header info
	outputFile << "Magic ID: " << m_header.magic << std::endl;
    outputFile << "unk: " << m_header.unk << std::endl;
    outputFile << "unk 2: " << m_header.unk2 << std::endl;
    outputFile << "String table offset: 0x" << std::hex << m_header.stringTableOffset << std::dec << std::endl;
    outputFile << "World scale: " << m_header.worldScale << std::endl;
    outputFile << "Terrain mesh max height: " << m_header.terrainMeshMaxHeight << std::endl;
    outputFile << "Material info array length: " << m_header.materialInfoArrayLength << std::endl;
    outputFile << "Area array length: " << m_header.areaArrayLength << std::endl;
    outputFile << "Padding: " << m_header.padding << std::endl;
    outputFile << "Padding 2: " << m_header.padding2 << std::endl;
    outputFile << "Tile size: " << m_header.tileSize << std::endl;
    outputFile << "unk 3: " << m_header.unk3 << std::endl;
    outputFile << "Size of Material Lookup Table: 0x" << std::hex << m_header.materialLookupTableSize << std::dec << std::endl << std::endl;

    // write material table info
    for (int i = 0; i < m_header.materialInfoArrayLength; i++)
    {
        outputFile << "Material offset: 0x" << std::hex << m_materialTableOffsets[i] << std::dec << std::endl;
        outputFile << "Material index: " << m_materialTable[i].index << std::endl;
        outputFile << "Material Texture U Axis: " << m_materialTable[i].textureUAxis << std::endl;
        outputFile << "Material Texture V axis: " << m_materialTable[i].textureVAxis << std::endl;
        outputFile << "Material unknown: " << m_materialTable[i].unk << std::endl;
        outputFile << "Material unknown 2: " << m_materialTable[i].unk2 << std::endl << std::endl;
    }

    // write area table info
    for (int i = 0; i < m_header.areaArrayLength; i++)
    {
        //outputFile << "Offset: 0x" << std::hex << m_areaTableOffsets[i] << std::dec << std::endl;
        outputFile << "Area Name: " << m_areaNames[i] << std::endl;
        outputFile << "X Position: " << m_areaTable[i].xPosition << std::endl;
        outputFile << "Z Position: " << m_areaTable[i].zPosition << std::endl;
        outputFile << "Area Size: " << m_areaTable[i].areaSize<< std::endl;
        outputFile << "Grass Density: " << m_areaTable[i].grassDensity << std::endl << std::endl;
        /*outputFile << "unk: " << m_areaTable[i].unk << std::endl;
        m_outputFile << "unk2: " << m_areaTable[i].unk2 << std::endl;
        m_outputFile << "unk3: " << m_areaTable[i].unk3 << std::endl;
        m_outputFile << "unk4: " << m_areaTable[i].unk4 << std::endl;
        m_outputFile << "Name Offset: 0x" << std::hex << m_areaTable[i].nameOffset << std::dec << std::endl;
        m_outputFile << "unk5: " << m_areaTable[i].unk5 << std::endl;
        m_outputFile << "unk6: " << m_areaTable[i].unk6 << std::endl;
        m_outputFile << "unk7: " << m_areaTable[i].unk7 << std::endl << std::endl;*/
    }

    outputFile.close();
}

bool vertSort(const glm::vec3& a, const glm::vec3& b)
{
	if (a.y < b.y)
		return true;

	return a.x < b.x;
}

void TSCB::writeCombinedOBJ(int LOD, const std::string& filename)
{
	std::vector<std::string> lodFilenames = getFileNamesByLOD(LOD);

	std::vector<glm::vec3> verts;

	for (int i = 0; i < lodFilenames.size(); i++)
	{
		glm::vec2 offset = TSCB::getInstance().getAreaLocation(lodFilenames[i]) * 256.0f;
		HGHT currentHGHt(filenameUtils::getHGHTFolder() + "/" + lodFilenames[i] + ".hght");
		TSCBArea area = getAreabyName(lodFilenames[i]);
		float scale = (area.areaSize / m_header.tileSize * m_header.worldScale) * 20.0f;

		for (int y = 0; y < 256; y++)
		{
			for (int x = 0; x < 256; x++)
			{
				verts.push_back(glm::vec3(offset.x + x, currentHGHt.getHeight(y, x) / scale, offset.y + y));
			}
		}
	}

	// all verts are in the vector so sort it
	std::sort(verts.begin(), verts.end(), vertSort);

	// open file
	std::ofstream file;
	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to load " << filename << std::endl;
		return;
	}

	// write object header
	file << "o model" << std::endl;

	for (int i = 0; i < verts.size(); i++)
	{
		file << "v " << verts[i].x << " " << verts[i].y << " " << verts[i].z << std::endl;
	}

	file.close();
}

// returns every filename
std::vector<std::string> TSCB::getAllFileNames()
{
	return m_areaNames;
}

bool sortByPosition(const std::string& a, const std::string& b)
{
	glm::vec2 posa = TSCB::getInstance().getAreaLocation(a);
	glm::vec2 posb = TSCB::getInstance().getAreaLocation(b);

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

	for (auto iter = m_areaNames.begin(); iter != m_areaNames.end(); iter++)
	{
		if ((*iter)[1] == c)
		{
			filenames.push_back(*iter);
		}
	}

	std::sort(filenames.begin(), filenames.end(), sortByPosition);

	return filenames;
}

TSCBArea TSCB::getAreabyName(const std::string& name)
{
	for (size_t i = 0; i < m_areaNames.size(); i++)
	{
		if (m_areaNames[i] == name)
		{
			return m_areaTable[i];
		}
	}

	return m_areaTable[0];
}

// get the position for area with filename
glm::vec2 TSCB::getAreaLocation(const std::string& filename)
{
	TSCBArea area = getAreabyName(filename);

	return glm::vec2(area.xPosition, area.zPosition);
}

// adjust area positionX and positionZ values to start at the top left corner
void TSCB::fixAreaOffsets()
{
	int currentLOD = 0;

	for (size_t i = 0; i < m_areaTable.size(); i++)
	{
		if (m_areaNames[i].substr(1, 1) != std::to_string(currentLOD))
		{
			currentLOD++;
		}

		m_areaTable[i].xPosition -= lowest[currentLOD].x;
		m_areaTable[i].xPosition /= m_areaTable[i].areaSize;
		m_areaTable[i].zPosition -= lowest[currentLOD].y;
		m_areaTable[i].zPosition /= m_areaTable[i].areaSize;
	}
}
