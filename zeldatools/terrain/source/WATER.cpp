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

Texture2D WATER::getTexture() const
{
	Texture2D texture(64, 64, 3);

	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			WATERInfo currentInfo = m_waterInfo(y, x);
			Color currentColor = TextureHolder::getInstance().getWaterPixelApproximate(currentInfo);

			texture.setPixel(x, y, currentColor);
		}
	}

	return texture;
}

// write png
void WATER::writeTexture(const std::string& filename)
{
	getTexture().save(filename.c_str());
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
