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
	m_filename = filename;
	readGrass();
}

void GRASS::load(const std::string& filename)
{
	m_filename = filename;
	readGrass();
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

// save as png
void GRASS::writeTexture(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[64][64][3];

	// set values in pixel buffer
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			pixelData[x][y][0] = m_grassInfo(x, y).r;
			pixelData[x][y][1] = m_grassInfo(x, y).g;
			pixelData[x][y][2] = m_grassInfo(x, y).b;
		}
	}

	Texture2D tex(&pixelData[0][0][0], 64, 64, 3);
	tex.save(filename.c_str());
}

void GRASS::writeRaw(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[64][64][4];

	// set values in pixel buffer
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			GRASSinfo currentGrassInfo = m_grassInfo(x, y);

			pixelData[x][y][0] = currentGrassInfo.r;
			pixelData[x][y][1] = currentGrassInfo.g;
			pixelData[x][y][2] = currentGrassInfo.b;
			// use alpha for height for readability
			pixelData[x][y][3] = currentGrassInfo.BladeHeight;
		}
	}

	Texture2D tex(&pixelData[0][0][0], 64, 64, 4);
	tex.save(filename.c_str());
}

void GRASS::dumpGRASSFile(const std::string& filename)
{
	GRASS grass(filename);

	std::string outputFolder = filenameUtils::getGRASSFolder() + "\\images\\" + fs::path(filename).stem().string()[1] + "\\";

	if (!fs::exists(outputFolder))
	{
		fs::create_directory(outputFolder);
	}

	if (!fs::exists(outputFolder + fs::path(filename).stem().string()))
	{
		grass.writeTexture(outputFolder + fs::path(filename).stem().string());
	}
}

void GRASS::setTestValues()
{
	for (size_t x = 0; x < 64; x++)
	{
		for (size_t y = 0; y < 64; y++)
		{
			m_grassInfo(x, y).r = 128;
			m_grassInfo(x, y).g = 0;
			m_grassInfo(x, y).b = 255;
			m_grassInfo(x, y).BladeHeight = 0;
		}
	}
}

void GRASS::readGrass()
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
			file.read(reinterpret_cast<char*>(&m_grassInfo(x, y)), sizeof(GRASSinfo));
		}
	}

	// close the file
	file.close();
}
