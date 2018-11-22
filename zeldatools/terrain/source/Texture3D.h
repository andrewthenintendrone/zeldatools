#pragma once
#include "Color.h"

class Texture3D
{
public:

	Texture3D() {};
	Texture3D(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int format);

	void bind(unsigned int slot) const;

	unsigned int getHandle() const { return m_glHandle; }

private:

	unsigned int m_width = 0;
	unsigned int m_height = 0;
	unsigned int m_depth = 0;
	unsigned int m_format = 0;
	unsigned int m_glHandle = 0;

	unsigned char* m_buffer = nullptr;
};