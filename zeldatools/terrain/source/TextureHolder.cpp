#include "TextureHolder.h"
#include <iostream>
#include <fstream>
#include "endian.h"
#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;


TextureHolder& TextureHolder::getInstance()
{
	static TextureHolder instance;
	return instance;
}

TextureHolder::TextureHolder()
{
	if (shouldLoadTextures)
	{
		loadTextures();
	}
}

TextureHolder::~TextureHolder()
{
	
}

void TextureHolder::loadTextures()
{
	// load terrain textures from materialalb.dds
	std::fstream terrainFile;
	terrainFile.open(fs::current_path().string() + std::string("\\materialalb.dds"), std::ios::binary | std::ios::_Nocreate);

	if (!terrainFile.is_open())
	{
		std::cout << "Failed to open materialalb.dds" << std::endl;
	}

	// skip dds header
	terrainFile.seekg(0x80);

	unsigned char* terrainBuffer = new unsigned char[1024 * 1024 * 4 * 83];
	terrainFile.read(reinterpret_cast<char*>(terrainBuffer), (1024 * 1024 * 4 * 83));

	m_terrainTextures = Texture3D(terrainBuffer, 1024, 1024, 83, GL_RGBA);

	terrainFile.close();

	// load water textures from wateremm.dds
	std::fstream waterFile;
	waterFile.open(fs::current_path().string() + std::string("\\wateremm.dds"), std::ios::binary | std::ios::_Nocreate);

	if (!waterFile.is_open())
	{
		std::cout << "Failed to open wateremm.dds" << std::endl;
	}

	// skip dds header
	waterFile.seekg(0x80);

	unsigned char* waterBuffer = new unsigned char[512 * 512 * 4 * 8];
	waterFile.read(reinterpret_cast<char*>(waterBuffer), (512 * 512 * 4 * 8));

	m_waterTextures = Texture3D(waterBuffer, 512, 512, 8, GL_RGBA);

	waterFile.close();

	shouldLoadTextures = false;
}

Color TextureHolder::getTerrainPixel(const MATEinfo& mateInfo, int x, int y)
{
	//// fix material indices
	//uint8_t m1 = fixMaterialIndex(mateInfo.id1);
	//uint8_t m2 = fixMaterialIndex(mateInfo.id2);

	//// sample from textures
	//Color c1 = m_terrainTextures[m1]->getPixel(x * 8, y * 8);
	//Color c2 = m_terrainTextures[m2]->getPixel(x * 8, y * 8);

	//return Color::lerp(c1, c2, (float)mateInfo.blend / 255.0f);

	return Color();
}

Color TextureHolder::getTerrainPixelApproximate(const MATEinfo& mateInfo, int x, int y)
{
	// get both colors
	Color c1 = m_approximateTerrainColors[mateInfo.id1];
	Color c2 = m_approximateTerrainColors[mateInfo.id2];

	return Color::lerp(c1, c2, mateInfo.blend / 255.0f);
}

Color TextureHolder::getWaterPixel(const WATERInfo& waterInfo, int x, int y)
{
	return Color(0x000000ff);
}

Color TextureHolder::getWaterPixelApproximate(const WATERInfo& waterInfo)
{
	return m_approximateWaterColors[waterInfo.materialIndex];
}

void TextureHolder::bindTextures(Shader shader)
{
	shader.setInt("terrainTextures", 0);
	m_terrainTextures.bind(0);

	shader.setInt("waterTextures", 1);
	m_waterTextures.bind(1);
}

