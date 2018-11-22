#pragma once
#include "Color.h"

class Texture2D
{
public:

	Texture2D();
	Texture2D(const char* filename);
	Texture2D(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int format);
	~Texture2D();

	void load(const char* filename);

	void bind(unsigned int slot) const;

	unsigned int getHandle() const { return m_glHandle; }

	Color getPixel(int x, int y);

private:

	unsigned int m_width = 0;
	unsigned int m_height = 0;
	unsigned int m_format = 0;
	unsigned int m_glHandle = 0;

	unsigned char* m_buffer = nullptr;
};