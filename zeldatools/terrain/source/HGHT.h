#pragma once
#include <string>
#include "Array2D.h"
#include "Texture2D.h"

class HGHT
{
public:

#pragma region constructors

	HGHT() {};
	HGHT(const std::string& filename);

#pragma endregion

	void load(const std::string& filename);
	void save(const std::string& filename);

	Texture2D getTexture() const;

	// write files
	void writeTexture(const std::string& filename);
	void writeOBJ(const std::string& filename);
	void writeOBJPointCloud(const std::string& filename);

	// sample height
	uint16_t getHeight(unsigned int x, unsigned int y) { return m_heights(x, y); }

	void average();
	const uint16_t getHighestPoint();
	const uint16_t getLowestPoint();

	static void dumpHGHTFile(const std::string& filename);

	int getLOD() const;

	std::string getFilename() const
	{
		return m_filename;
	}

private:

	// filename
    std::string m_filename;

	// read heights from hght file
    void readHeights();

	// height array
	Array2D<uint16_t> m_heights = Array2D<uint16_t>(256, 256);
};