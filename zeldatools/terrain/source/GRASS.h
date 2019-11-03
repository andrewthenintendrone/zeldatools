#pragma once
#include <vector>
#include "Array2D.h"
#include <glm\glm.hpp>

struct GRASSinfo
{
	uint8_t BladeHeight;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class GRASS
{
public:

	GRASS() {};
	GRASS(const std::string& filename);

	void load(const std::string& filename);
	void save(const std::string& filename);

	// write files
	void writeTexture(const std::string& filename);
	void writeRaw(const std::string& filename);
	void writeUnknown(const std::string& filename);

	const GRASSinfo getGrassInfo(unsigned int x, unsigned int y) const { return m_grassInfo(x, y); }
	GRASSinfo& getGrassInfo(unsigned int x, unsigned int y) { return m_grassInfo(x, y); }

	static void dumpGRASSFile(const std::string& filename);

	void setTestValues();

private:

	std::string m_filename;

	void readGrass();

	// material info array
	Array2D<GRASSinfo> m_grassInfo = Array2D<GRASSinfo>(64, 64);
};