#include "MATE.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stb\stb_image_write.h>
#include "filenameUtils.h"
#include "TextureHolder.h"
#include "Texture2D.h"

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

void MATE::save(const std::string& filename)
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
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			file.write(reinterpret_cast<char*>(&m_mateInfo(x, y)), sizeof(MATEinfo));
		}
	}

	// close the file
	file.close();
}

// save as png
void MATE::writeTexture(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[256][256][3];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			Color pixelColor = TextureHolder::getInstance().getTerrainPixelApproximate(m_mateInfo(x, y), x, y);

			pixelData[x][y][0] = pixelColor.r;
			pixelData[x][y][1] = pixelColor.g;
			pixelData[x][y][2] = pixelColor.b;
		}
	}

	Texture2D tex(&pixelData[0][0][0], 256, 256, 3);
	tex.save(filename.c_str());
}

void MATE::writeRaw(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[256][256][4];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			MATEinfo currentMateInfo = m_mateInfo(x, y);

			pixelData[x][y][0] = currentMateInfo.id1;
			pixelData[x][y][1] = currentMateInfo.id2;
			pixelData[x][y][2] = currentMateInfo.blend;
			pixelData[x][y][3] = currentMateInfo.unk;
		}
	}

	Texture2D tex(&pixelData[0][0][0], 256, 256, 4);
	tex.save(filename.c_str());
}

void MATE::writeUnknown(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[256][256];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			MATEinfo currentMateInfo = m_mateInfo(x, y);

			pixelData[x][y] = currentMateInfo.unk;
		}
	}

	Texture2D tex(&pixelData[0][0], 256, 256, 1);
	tex.save(filename.c_str());
}

void MATE::dumpMATEFile(const std::string& filename)
{
	MATE mate(filename);

	std::string outputFolder = filenameUtils::getMATEFolder() + "\\images\\" + fs::path(filename).stem().string()[1] + "\\";

	if (!fs::exists(outputFolder))
	{
		fs::create_directory(outputFolder);
	}

	if (!fs::exists(outputFolder + fs::path(filename).stem().string()))
	{
		mate.writeTexture(outputFolder + fs::path(filename).stem().string());
	}
}

void MATE::setTestValues()
{
	for (size_t x = 0; x < 256; x++)
	{
		for (size_t y = 0; y < 256; y++)
		{
			m_mateInfo(x, y).id1 = 64;
			m_mateInfo(x, y).id2 = 0;
			m_mateInfo(x, y).blend = 0;
			m_mateInfo(x, y).unk = 128;
		}
	}
}

void MATE::setUnknown(uint8_t value)
{
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			m_mateInfo(x, y).unk = value;
		}
	}
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
