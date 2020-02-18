#include "Texture2D.h"
#include <iostream>
#include "glad\glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb\stb_image_write.h"

#pragma region constructors

Texture2D::Texture2D(const Texture2D& other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;
	m_filename = other.m_filename;
	m_pixelBuffer = new unsigned char[m_width * m_height * m_channels];

	for (size_t i = 0; i < (m_width * m_height * m_channels); i++)
	{
		m_pixelBuffer[i] = other.m_pixelBuffer[i];
	}
}

Texture2D::Texture2D(int width, int height, int channels, unsigned char* buffer)
{
	m_width = width;
	m_height = height;
	m_channels = channels;

	if (buffer)
	{
		m_pixelBuffer = buffer;
	}
	else
	{
		m_pixelBuffer = new unsigned char[m_width * m_height * m_channels];
	}
}

Texture2D::Texture2D(const std::string& filename)
{
	load(filename);
}

Texture2D::~Texture2D()
{
	if (m_pixelBuffer != nullptr)
	{
		stbi_image_free(m_pixelBuffer);
		m_pixelBuffer = nullptr;
	}
}

#pragma endregion

void Texture2D::load(const std::string& filename)
{
	// attempt to load image
	m_pixelBuffer = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);

	// check that image was loaded
	if (m_pixelBuffer)
	{
		m_filename = filename;
	}
	else
	{
		std::cout << "failed to load " << filename << std::endl;
		stbi_image_free(m_pixelBuffer);
		m_pixelBuffer = nullptr;
	}
}

void Texture2D::save(const std::string& filename)
{
	stbi_write_png(filename.c_str(), m_width, m_height, m_channels, m_pixelBuffer, m_width * m_channels);
}

void Texture2D::generate()
{
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int format = (m_channels == 4 ? GL_RGBA : GL_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_pixelBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

//void Texture2D::initialize()
//{
//	// clean up
//	if (m_glHandle != 0)
//	{
//		glDeleteTextures(1, &m_glHandle);
//	}
//
//	glGenTextures(1, &m_glHandle);
//	glBindTexture(GL_TEXTURE_2D, m_glHandle);
//
//	switch (m_channels)
//	{
//	case STBI_grey:
//		m_format = GL_ALPHA;
//		break;
//	case STBI_grey_alpha:
//		m_format = GL_RG;
//		break;
//	case STBI_rgb:
//		m_format = GL_RGB;
//		break;
//	case STBI_rgb_alpha:
//		m_format = GL_RGBA;
//		break;
//	default:
//		break;
//	};
//
//	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_pixelBuffer);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

//void Texture2D::bind(unsigned int slot) const
//{
//	glActiveTexture(GL_TEXTURE0 + slot);
//	glBindTexture(GL_TEXTURE_2D, m_glHandle);
//}

Color Texture2D::getPixel(int x, int y) const
{
	unsigned int pixelOffset = getPixelOffset(x, y);

	Color pixel;

	pixel.r = m_pixelBuffer[pixelOffset];
	pixel.g = m_pixelBuffer[pixelOffset + 1];
	pixel.b = m_pixelBuffer[pixelOffset + 2];

	// alpha
	if (m_channels == 4)
	{
		pixel.a = m_pixelBuffer[pixelOffset + 3];
	}

	return pixel;
}

void Texture2D::setPixel(int x, int y, Color color)
{
	unsigned int pixelOffset = getPixelOffset(x, y);

	m_pixelBuffer[pixelOffset] = color.r;
	m_pixelBuffer[pixelOffset + 1] = color.g;
	m_pixelBuffer[pixelOffset + 2] = color.b;

	// alpha
	if (m_channels == 4)
	{
		m_pixelBuffer[pixelOffset + 3] = color.a;
	}
}

void Texture2D::operator = (Texture2D& other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;

	if (m_pixelBuffer)
	{
		delete[] m_pixelBuffer;
		m_pixelBuffer = nullptr;
	}

	m_pixelBuffer = new unsigned char[m_width * m_height * m_channels];

	for (int i = 0; i < (m_height * m_width * m_channels); i++)
	{
		m_pixelBuffer[i] = other.m_pixelBuffer[i];
	}
}

int Texture2D::getPixelOffset(int x, int y) const
{
	if (x < 0 || y < 0 || x > m_width - 1 || y > m_height - 1)
	{
		std::cout << "Tried to access pixel outside of Image" << std::endl;
		return -1;
	}

	return (m_channels * (y * m_width + x));
}
