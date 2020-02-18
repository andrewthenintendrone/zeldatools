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

// create a texture from this material map
Texture2D MATE::getTexture() const
{
	Texture2D texture(256, 256, 3);

	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			MATEinfo currentInfo = m_mateInfo(y, x);
			Color currentColor = TextureHolder::getInstance().getTerrainPixelApproximate(currentInfo, x, y);
			texture.setPixel(x, y, currentColor);
		}
	}

	return texture;
}

// save as png
void MATE::writeTexture(const std::string& filename)
{
	getTexture().save(filename.c_str());
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

	Texture2D tex(256, 256, 4, &pixelData[0][0][0]);
	tex.save(filename.c_str());
}

int MATE::fixMaterialIndex(int index)
{
	if (index == 29)
	{
		return 17;
	}
	if (index == 30)
	{
		return 18;
	}
	if (index == 31 || index == 76)
	{
		return 0;
	}
	if (index > 31 && index < 63)
	{
		return index - 3;
	}
	if (index == 63)
	{
		return 7;
	}
	if (index > 63 && index < 76)
	{
		return index - 4;
	}
	if (index > 76)
	{
		return index - 5;
	}

	return index;
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

	Texture2D tex(256, 256, 1, &pixelData[0][0]);
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
			m_mateInfo(x, y).id1 = fixMaterialIndex(m_mateInfo(x, y).id1);
			m_mateInfo(x, y).id2 = fixMaterialIndex(m_mateInfo(x, y).id2);
		}
	}

	// close the file
	file.close();
}
