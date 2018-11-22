#pragma once
#include <string>
#include "Array2D.h"

class HGHT
{
public:

	HGHT() {};
    HGHT(const std::string& filename);

	void load(const std::string& filename);

	// write files
	void writeTexture(const std::string& filename);
	void writeOBJ(const std::string& filename);

	// sample height
	uint16_t getHeight(unsigned int x, unsigned int y) { return m_heights(x, y); }

	const int getLOD();

private:

	// filename
    std::string m_filename;

	// read heights from hght file
    void readHeights();

	// height array
	Array2D<uint16_t> m_heights = Array2D<uint16_t>(256, 256);
};