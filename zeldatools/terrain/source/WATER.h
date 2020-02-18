#pragma once
#include "Array2D.h"
#include "Texture2D.h"

struct WATERInfo
{
	uint16_t height;
	uint16_t flowRateX;
	uint16_t flowRateZ;
	uint8_t materialIndexPlus3;
	uint8_t materialIndex;
};

class WATER
{
public:

	WATER() {};
	WATER(const std::string& filename);

	void load(const std::string& filename);

	Texture2D getTexture() const;

	// write files
	void writeTexture(const std::string& filename);

	WATERInfo getWaterInfo(int x, int y) const { return m_waterInfo(x, y); }

	uint16_t getHeight(int x, int y) const { return m_waterInfo(x, y).height; }

private:

	std::string m_filename;

	void readWaterinfo();

	Array2D<WATERInfo> m_waterInfo = Array2D<WATERInfo>(64, 64);
};