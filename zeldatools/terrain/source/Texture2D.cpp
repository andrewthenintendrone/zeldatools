#include "Texture2D.h"
#include <iostream>
#include "glad\glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(const char* filename)
{
	load(filename);
}

Texture2D::Texture2D(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int format)
{
	m_buffer = buffer;
	m_width = width;
	m_height = height;
	m_format = format;

	glGenTextures(1, &m_glHandle);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	if (m_glHandle != 0)
	{
		glDeleteTextures(1, &m_glHandle);
	}

	if (m_buffer)
	{
		stbi_image_free(m_buffer);
		m_buffer = nullptr;
	}
}

void Texture2D::load(const char* filename)
{
	int x = 0, y = 0, comp = 0;

	m_buffer = stbi_load(filename, &x, &y, &comp, STBI_default);

	if (m_buffer)
	{
		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);

		switch (comp)
		{
		case STBI_grey:
			m_format = GL_ALPHA;
			break;
		case STBI_grey_alpha:
			m_format = GL_RG;
			break;
		case STBI_rgb:
			m_format = GL_RGB;
			break;
		case STBI_rgb_alpha:
			m_format = GL_RGBA;
			break;
		default:
			break;
		};

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, x, y, 0, m_format, GL_UNSIGNED_BYTE, m_buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_width = (unsigned int)x;
		m_height = (unsigned int)y;
	}
	else
	{
		std::cout << "failed to load " << filename << std::endl;
	}
}

void Texture2D::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_glHandle);
}

Color Texture2D::getPixel(int x, int y)
{
	x = x % m_width;
	y = y % m_height;

	if (m_format == GL_RGB)
	{
		stbi_uc red = m_buffer[y * m_width * 3 + x * 3];
		stbi_uc green = m_buffer[y * m_width * 3 + x * 3 + 1];
		stbi_uc blue = m_buffer[y * m_width * 3 + x * 3 + 2];

		return Color(red, green, blue, 255);
	}
	else if (m_format == GL_RGBA)
	{
		stbi_uc red = m_buffer[y * m_width * 4 + x * 4];
		stbi_uc green = m_buffer[y * m_width * 4 + x * 4 + 1];
		stbi_uc blue = m_buffer[y * m_width * 4 + x * 4 + 2];
		stbi_uc alpha = m_buffer[y * m_width * 4 + x * 4 + 3];

		return Color(red, green, blue, alpha);
	}
	else
	{
		std::cout << "unsupported number of channels" << std::endl;
		return Color();
	}
}
