#pragma once
#include <string>

#include "Color.h"

class Texture2D
{
public:

	#pragma region constructors

	Texture2D() {};
	Texture2D(const Texture2D& other);
	Texture2D(int width, int height, int channels, unsigned char* buffer = nullptr);
	Texture2D(const std::string& filename);
	~Texture2D();

	#pragma endregion

	void load(const std::string& filename);
	void save(const std::string& filename);

	void generate();
	void bind() const;

	Color getPixel(int x, int y) const;
	void setPixel(int x, int y, Color color);

	void operator = (Texture2D& other);

private:

	std::string m_filename;
	unsigned char* m_pixelBuffer = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;

	unsigned int m_handle = 0;

	int getPixelOffset(int x, int y) const;
};