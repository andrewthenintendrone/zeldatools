#pragma once
#include <string>
#include <fstream>
#include "Color.h"
#include <vector>
#include "MATE.h"
#include "WATER.h"
#include "Texture3D.h"
#include "Shader.h"

class TextureHolder
{
public:

	static TextureHolder& getInstance();

	Color getTerrainPixel(const MATEinfo& mateInfo, int x, int y);
	Color getTerrainPixelApproximate(const MATEinfo& mateInfo, int x, int y);

	Color getWaterPixel(const WATERInfo& waterInfo, int x, int y);
	Color getWaterPixelApproximate(const WATERInfo& waterInfo);

	void bindTextures(Shader shader);

private:

	TextureHolder();
	~TextureHolder();

	void loadTextures();

	bool shouldLoadTextures = false;

	std::ifstream m_inputFile;

	Texture3D m_terrainTextures;
	Texture3D m_waterTextures;

	std::vector<Color> m_approximateTerrainColors
	{
		Color(0x4c6436ff),
		Color(0x5e5d52ff),
		Color(0x7d5e2bff),
		Color(0x504f4dff),
		Color(0x696968ff),
		Color(0x353f32ff),
		Color(0x796859ff),
		Color(0x817c6cff),
		Color(0x959493ff),
		Color(0x514e46ff),
		Color(0x75644fff),
		Color(0x4b563bff),
		Color(0x414244ff),
		Color(0x5f5e45ff),
		Color(0x3f3423ff),
		Color(0x414036ff),
		Color(0x545f47ff),
		Color(0x505151ff),
		Color(0x6c6e64ff),
		Color(0x7a635bff),
		Color(0x7d7363ff),
		Color(0x85663aff),
		Color(0x52514fff),
		Color(0x7b5e54ff),
		Color(0x241810ff),
		Color(0x3e3e38ff),
		Color(0x59694aff),
		Color(0x49342dff),
		Color(0x826f4eff),
		Color(0x6b622fff),
		Color(0x3a332fff),
		Color(0x5d423aff),
		Color(0x36251cff),
		Color(0xa29e9eff),
		Color(0x5d4d21ff),
		Color(0x403830ff),
		Color(0x262c1bff),
		Color(0x1e2a13ff),
		Color(0x2a3d15ff),
		Color(0x36420fff),
		Color(0x554b41ff),
		Color(0x225067ff),
		Color(0x4f4b3dff),
		Color(0x5d564fff),
		Color(0x6a6a6dff),
		Color(0x6b4335ff),
		Color(0x616161ff),
		Color(0x2f2d2aff),
		Color(0x2e3f4cff),
		Color(0xae9a7bff),
		Color(0x7b858aff),
		Color(0x53626aff),
		Color(0x3a3b3cff),
		Color(0x827b69ff),
		Color(0x8e7868ff),
		Color(0x31532cff),
		Color(0x867463ff),
		Color(0x4b4d44ff),
		Color(0x4f4f3fff),
		Color(0x968d69ff),
		Color(0x35546aff),
		Color(0x595b5aff),
		Color(0x656054ff),
		Color(0x2d3c22ff),
		Color(0x67403dff),
		Color(0x2f341fff),
		Color(0x81554eff),
		Color(0xb09466ff),
		Color(0x726256ff),
		Color(0x707871ff),
		Color(0x6a4641ff),
		Color(0x62605dff),
		Color(0x35431eff),
		Color(0x7b6e5eff),
		Color(0x5a6945ff),
		Color(0x2e302cff),
		Color(0x75726aff),
		Color(0x443f38ff),
		Color(0x5f5c4bff),
		Color(0x3c492aff),
		Color(0x61542eff),
		Color(0x9c7a77ff),
		Color(0x211e1bff)
	};

	std::vector<Color> m_approximateWaterColors
	{
		Color::Aqua(),
		Color::Aquamarine(),
		Color::Purple(),
		Color::Red(),
		Color::Cyan(),
		Color::Brown(),
		Color::White(),
		Color::LightSeaGreen()
	};
};
