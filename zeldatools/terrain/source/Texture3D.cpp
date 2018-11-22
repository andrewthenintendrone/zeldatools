#include "Texture3D.h"
#include "glad\glad.h"

Texture3D::Texture3D(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int depth, unsigned int format)
{
	m_buffer = buffer;
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_format = format;

	// create opengl texture
	glGenTextures(1, &m_glHandle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_glHandle);

	// Allocate the storage.
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, depth);

	// Upload pixel data.
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);

	// Always set reasonable texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, m_format, m_width, m_height, m_depth, 0, m_format, GL_UNSIGNED_BYTE, buffer);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture3D::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_glHandle);
}
