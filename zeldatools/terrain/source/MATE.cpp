#include "MATE.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stb\stb_image_write.h>
#include "filenameUtils.h"
#include "TextureHolder.h"

MATE::MATE(const std::string& filename)
{
	m_filename = filename;
	readMaterial();
}

void MATE::load(const std::string& filename)
{
	m_filename = filename;
	readMaterial();
}

// save as png
void MATE::writeTexture(const std::string& filename)
{
	// create pixel buffer
	char pixelData[256][256][3];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			Color pixelColor = TextureHolder::getInstance().getTerrainPixel(m_mateInfo(x, y), x, y);

			pixelData[x][y][0] = pixelColor.r;
			pixelData[x][y][1] = pixelColor.g;
			pixelData[x][y][2] = pixelColor.b;
		}
	}

	// write file
	stbi_write_png((filename + ".png").c_str(), 256, 256, 3, pixelData, 256 * 3);
}

void MATE::writeRaw(const std::string& filename)
{
	// create pixel buffer
	char pixelData[256][256][3];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			MATEinfo currentMateInfo = m_mateInfo(x, y);

			pixelData[x][y][0] = currentMateInfo.unk;
			pixelData[x][y][1] = currentMateInfo.unk;
			pixelData[x][y][2] = currentMateInfo.unk;
		}
	}

	// write file
	stbi_write_png((filename + ".png").c_str(), 256, 256, 3, pixelData, 256 * 3);
}

void MATE::readMaterial()
{
	// attempt to open file
	std::ifstream file;
	file.open(m_filename, std::ios_base::binary | std::ios_base::_Nocreate);

	if (!file.is_open())
	{
		std::cout << "failed to open " << m_filename << std::endl;
		file.close();
		return;
	}

	// read file contents
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			file.read(reinterpret_cast<char*>(&m_mateInfo(x, y)), sizeof(MATEinfo));
		}
	}

	// close the file
	file.close();
}
