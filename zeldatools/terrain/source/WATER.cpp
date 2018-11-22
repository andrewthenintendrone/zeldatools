#include "WATER.h"
#include <string>
#include <iostream>
#include <fstream>
#include "TextureHolder.h"

#include <stb\stb_image_write.h>

WATER::WATER(const std::string& filename)
{
	m_filename = filename;
	readWaterinfo();
}

void WATER::load(const std::string& filename)
{
	m_filename = filename;
	readWaterinfo();
}

// write png
void WATER::writeTexture(const std::string& filename)
{
	// create pixel buffer
	char pixelData[64][64][3];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			Color pixelColor = TextureHolder::getInstance().getWaterPixel(m_waterInfo(x, y), x, y);

			pixelData[x][y][0] = pixelColor.r;
			pixelData[x][y][1] = pixelColor.g;
			pixelData[x][y][2] = pixelColor.b;
		}
	}

	// write file
	stbi_write_png((filename + ".png").c_str(), 256, 256, 3, pixelData, 256 * 3);
}

void WATER::readWaterinfo()
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
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			file.read(reinterpret_cast<char*>(&m_waterInfo(x, y)), sizeof(WATERInfo));
		}
	}

	// close the file
	file.close();
}
