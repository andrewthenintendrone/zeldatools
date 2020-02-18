#pragma once
#include <vector>
#include "Array2D.h"
#include <glm\glm.hpp>
#include "Texture2D.h"

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

	Texture2D getTexture(bool alpha = false) const;

	// write files
	void writeTexture(const std::string& filename, bool alpha = false);

	const GRASSinfo getGrassInfo(unsigned int x, unsigned int y) const { return m_grassInfo(x, y); }
	GRASSinfo& getGrassInfo(unsigned int x, unsigned int y) { return m_grassInfo(x, y); }

private:

	std::string m_filename;

	// material info array
	Array2D<GRASSinfo> m_grassInfo = Array2D<GRASSinfo>(64, 64);
};