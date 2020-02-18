#include "GRASS.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stb\stb_image_write.h>
#include "filenameUtils.h"
#include "TextureHolder.h"
#include "Texture2D.h"

GRASS::GRASS(const std::string& filename)
{
	load(filename);
}

void GRASS::load(const std::string& filename)
{
	m_filename = filename;

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
			file.read(reinterpret_cast<char*>(&m_grassInfo(x, y)), sizeof(GRASSinfo));
		}
	}

	// close the file
	file.close();
}

void GRASS::save(const std::string& filename)
{
	// attempt to open file
	std::ofstream file;
	file.open(filename, std::ios_base::binary | std::ios_base::trunc);

	if (!file.is_open())
	{
		std::cout << "failed to open " << filename << std::endl;
		file.close();
		return;
	}

	// write file contents
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			file.write(reinterpret_cast<char*>(&m_grassInfo(x, y)), sizeof(GRASSinfo));
		}
	}

	// close the file
	file.close();
}

Texture2D GRASS::getTexture(bool alpha) const
{
	Texture2D texture(64, 64, alpha ? 4 : 3);

	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			GRASSinfo currentInfo = m_grassInfo(y, x);
			texture.setPixel(x, y, Color(currentInfo.r, currentInfo.g, currentInfo.b, currentInfo.BladeHeight));
		}
	}

	return texture;
}

// save as png
void GRASS::writeTexture(const std::string& filename, bool alpha)
{
	getTexture(alpha).save(filename.c_str());
}
