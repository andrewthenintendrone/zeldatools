#pragma once
#include <vector>
#include <string>
#include <glm\glm.hpp>
#include "BinaryReader.h"
#include "Color.h"

// TSCB header struct
struct TSCBHeader
{
	char magic[5] = "    ";
	uint32_t unk = 0;
	uint32_t unk2 = 0;
	uint32_t stringTableOffset = 0;
	float worldScale = 0;
	float terrainMeshMaxHeight = 0;
	uint32_t materialInfoArrayLength = 0;
	uint32_t areaArrayLength = 0;
	uint32_t padding = 0;
	uint32_t padding2 = 0;
	float tileSize = 0;
	uint32_t unk3 = 0;
	uint32_t materialLookupTableSize = 0;
};

// TSCB material info struct
struct TSCBMaterialInfo
{
	uint32_t index = 0;
	float textureUAxis = 0;
	float textureVAxis = 0;
	float unk = 0;
	float unk2 = 0;
};

// TSCB Area struct
struct TSCBArea
{
	float xPosition = 0;
	float zPosition = 0;
	float areaSize = 0;
	float grassDensity = 0;
	float unk = 0;
	float unk2 = 0;
	float unk3 = 0;
	uint32_t unk4 = 0;
	uint32_t nameOffset = 0;
	uint32_t unk5 = 0;
	uint32_t unk6 = 0;
	uint32_t unk7 = 0;
};

// tsb file class
class TSCB
{
public:

	static TSCB& getInstance();

	void readFile();
	void writeFile();

	void writeCombinedOBJ(int LOD, const std::string& filename);

	std::vector<std::string> getAllFileNames();
	std::vector<std::string> getFileNamesByLOD(int LOD);

	TSCBHeader getHeader() const { return m_header; }

	TSCBArea getAreabyName(const std::string& filename);

	glm::vec2 getAreaLocation(const std::string& filename);

	glm::vec2 getSpan(int LOD) { return span[LOD]; }

private:

	TSCB();

	BinaryReader m_reader;

	// data
	TSCBHeader m_header;
	std::vector<uint32_t> m_materialTableOffsets;
	std::vector<TSCBMaterialInfo> m_materialTable;
	std::vector<uint32_t> m_areaTableOffsets;
	std::vector<TSCBArea> m_areaTable;
	std::vector<std::string> m_areaNames;

	void readHeader();
	void readMaterialTableOffsets();
	void readMaterialTable();
	void readAreaTableOffsets();
	void readAreaTable();
	void readAreaNames();

	void fixAreaOffsets();

	glm::vec2 lowest[9]
	{
		glm::vec2(0, 0),
		glm::vec2(-8, -8),
		glm::vec2(-12, -12),
		glm::vec2(-10, -10),
		glm::vec2(-11, -11),
		glm::vec2(-9.5, -7.5),
		glm::vec2(-9.75, -7.75),
		glm::vec2(-9.875, -7.625),
		glm::vec2(-9.8125, -7.6875)
	};

	glm::vec2 span[9]
	{
		glm::vec2(1, 1),
		glm::vec2(2, 2),
		glm::vec2(4, 4),
		glm::vec2(6, 6),
		glm::vec2(12, 12),
		glm::vec2(20, 16),
		glm::vec2(40, 32),
		glm::vec2(80, 63),
		glm::vec2(159, 124),
	};

	float m_edgeLengths[9]
	{
		32.0f,
		16.0f,
		8.0f,
		4.0f,
		2.0f,
		1.0f,
		0.5f,
		0.25f,
		0.125f,
	};
};