#pragma once
#include <vector>
#include "Array2D.h"
#include <glm\glm.hpp>

struct MATEinfo
{
	uint8_t id1;
	uint8_t id2;
	uint8_t blend;
	uint8_t unk;
};

class MATE
{
public:

	MATE() {};
	MATE(const std::string& filename);

	void load(const std::string& filename);
	void save(const std::string& filename);

	// write files
	void writeTexture(const std::string& filename);
	void writeRaw(const std::string& filename);
	void writeUnknown(const std::string& filename);

	const MATEinfo getMateInfo(unsigned int x, unsigned int y) const { return m_mateInfo(x, y); }
	MATEinfo& getMateInfo(unsigned int x, unsigned int y) { return m_mateInfo(x, y); }

	static void dumpMATEFile(const std::string& filename);

	void setTestValues();

	void setUnknown(uint8_t value);

private:

    std::string m_filename;

	void readMaterial();

	// material info array
	Array2D<MATEinfo> m_mateInfo = Array2D<MATEinfo>(256, 256);
};