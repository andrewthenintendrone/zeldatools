#include "HGHT.h"
#include <fstream>
#include <iostream>
#include <string>
#include "MATE.h"
#include "filenameUtils.h"
#include "TSCB.h"
#include "Texture2D.h"

HGHT::HGHT(const std::string& filename)
{
	m_filename = filename;

    readHeights();
}

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

// save as png (loses accuracy)
void HGHT::writeTexture(const std::string& filename)
{
	// create pixel buffer
	unsigned char pixelData[256][256][2];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			pixelData[x][y][0] = (m_heights(x, y) >> 8);
			pixelData[x][y][1] = (m_heights(x, y) & 0xFF);
		}
	}

	Texture2D tex(&pixelData[0][0][0], 256, 256, 2);
	tex.save(filename.c_str());
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

	// write verts
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			glm::vec3 currentPosition = glm::vec4(x, (float)m_heights(y, x) / 2048.0f, y, 1);

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

const int HGHT::getLOD()
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
			m_heights(x, y) = averageHeight;
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
