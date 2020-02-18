#include "HGHT.h"
#include <fstream>
#include <iostream>
#include <string>
#include "MATE.h"
#include "filenameUtils.h"
#include "TSCB.h"

#pragma region constructors

HGHT::HGHT(const std::string& filename)
{
	m_filename = filename;

	readHeights();
}

#pragma endregion

void HGHT::load(const std::string& filename)
{
	m_filename = filename;

	readHeights();
}

void HGHT::save(const std::string& filename)
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
			file.write(reinterpret_cast<char*>(&m_heights(x, y)), sizeof(uint16_t));
		}
	}

	// close the file
	file.close();
}

// create a texture from this heightmap
Texture2D HGHT::getTexture() const
{
	Texture2D texture(256, 256, 3);

	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			uint16_t currentHeight = m_heights(y, x);
			uint8_t currentColor = currentHeight >> 8;
			texture.setPixel(x, y, Color(currentColor, currentColor, currentColor));
		}
	}

	return texture;
}

// save as png (loses accuracy)
void HGHT::writeTexture(const std::string& filename)
{
	getTexture().save(filename.c_str());
}

void HGHT::writeOBJ(const std::string& filename)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to load " << filename << std::endl;
		return;
	}

	// write object header
	file << "o model" << std::endl;

	TSCBHeader header = TSCB::getInstance().getHeader();
	TSCBArea area = TSCB::getInstance().getAreabyName(filenameUtils::getFileNameOnly(m_filename));
	
	float scale = (area.areaSize / header.tileSize * header.worldScale) * 20.0f;
	glm::vec2 offset = TSCB::getInstance().getAreaLocation(filenameUtils::getFileNameOnly(m_filename)) * 256.0f;

	// write verts
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			glm::vec3 currentPosition = glm::vec3(offset.x + x, m_heights(y, x) / scale, offset.y + y);

			file << "v " << currentPosition.x << " " << currentPosition.y << " " << currentPosition.z << std::endl;
		}
	}

	for (int x = 0, i = 1; x < 256; x++)
	{
		for (int y = 0; y < 256; y++, i++)
		{
			// add indices
			if (x < 255 && y < 255)
			{
				file << "f " << i << " " << i + 256 << " " << i + 257 << std::endl;
				file << "f " << i << " " << i + 257 << " " << i + 1 << std::endl;
			}
		}
	}

	file.close();
}

void HGHT::writeOBJPointCloud(const std::string& filename)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to load " << filename << std::endl;
		return;
	}

	// write object header
	file << "o model" << std::endl;

	TSCBHeader header = TSCB::getInstance().getHeader();
	TSCBArea area = TSCB::getInstance().getAreabyName(filenameUtils::getFileNameOnly(m_filename));

	float scale = (area.areaSize / header.tileSize * header.worldScale) * 20.0f;
	glm::vec2 offset = TSCB::getInstance().getAreaLocation(filenameUtils::getFileNameOnly(m_filename)) * 256.0f;

	// write verts
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			glm::vec3 currentPosition = glm::vec3(offset.x + x, m_heights(y, x) / scale, offset.y + y);

			file << "v " << currentPosition.x << " " << currentPosition.y << " " << currentPosition.z << std::endl;
		}
	}

	file.close();
}

// read heights from file
void HGHT::readHeights()
{
	std::ifstream file;

	file.open(m_filename, std::ios::_Nocreate | std::ios::binary);

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
        	uint16_t currentHeight;

			file.read(reinterpret_cast<char*>(&currentHeight), sizeof(uint16_t));
			m_heights(x, y) = currentHeight;
        }
    }

    // close the file
	file.close();
}

int HGHT::getLOD() const
{
	return std::stoi(filenameUtils::getFileNameOnly(m_filename).substr(1, 1));
}

// averages heights
void HGHT::average()
{
	uint64_t averageHeight = 0;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			averageHeight += m_heights(x, y);
		}
	}

	averageHeight /= 65536;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			m_heights(x, y) = (uint16_t)averageHeight;
		}
	}
}

const uint16_t HGHT::getHighestPoint()
{
	uint16_t highestPoint = 0;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			highestPoint = std::max(highestPoint, m_heights(x, y));
		}
	}

	return highestPoint;
}

const uint16_t HGHT::getLowestPoint()
{
	uint16_t lowestPoint = 65535;

	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			lowestPoint = std::min(lowestPoint, m_heights(x, y));
		}
	}

	return lowestPoint;
}

void HGHT::dumpHGHTFile(const std::string& filename)
{
	HGHT hght(filename);

	std::string outputFolder = fs::path(filename).parent_path().string() + "\\images\\" + std::to_string(hght.getLOD()) + "\\";

	if (!fs::exists(outputFolder))
	{
		fs::create_directory(outputFolder);
	}

	// don't write file that already exists
	hght.writeTexture(outputFolder + fs::path(filename).stem().string());
}
