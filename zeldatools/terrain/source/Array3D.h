#pragma once
#include <vector>

// 3D array class
template<class T>
class Array3D
{
public:

	Array3D(unsigned int width, unsigned int height, unsigned int depth) : m_width(width), m_height(height), m_depth(depth), buffer(width * height * depth) {}

	const T& operator () (unsigned int x, unsigned int y, unsigned int z) const
	{
		return buffer[z * (m_width * m_height) + (y * m_width) + x];
	}

	T& operator() (unsigned int x, unsigned int y, unsigned int z)
	{
		return buffer[z * (m_width * m_height) + (y * m_width) + x];
	}


private:

	std::vector<T> buffer;

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_depth;
};
