#include "HGHT.h"
#include <fstream>
#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb\stb_image_write.h>
#include "MATE.h"
#include "filenameUtils.h"
#include "TSCB.h"

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

// save as png (loses accuracy)
void HGHT::writeTexture(const std::string& filename)
{
	// create pixel buffer
	char pixelData[256][256][3];

	// set values in pixel buffer
	for (int x = 0; x < 256; x++)
	{
		for (int y = 0; y < 256; y++)
		{
			char heightColor = m_heights(x, y) / 256;

			pixelData[x][y][0] = heightColor;
			pixelData[x][y][1] = heightColor;
			pixelData[x][y][2] = heightColor;
		}
	}

	// write file
	stbi_write_png((filename + ".png").c_str(), 256, 256, 3, pixelData, 256 * 3);
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
	m_heights = Array2D<uint16_t>(256, 256);

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
