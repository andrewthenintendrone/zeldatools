#pragma once
#include <vector>
#include <iostream>

template <class T>
class Array2D
{
public:

	Array2D(unsigned int width = 1, unsigned int height = 1) : m_width(width), m_height(height)
	{
		m_bufferSize = m_width * m_height;
		m_buffer = new T[m_bufferSize];
	}

	~Array2D()
	{
		delete[] m_buffer;
	}

	const unsigned int getWidth() const { return m_width; }
	const unsigned int getHeight() const { return m_height; }
	const unsigned int getBufferSize() const { return m_bufferSize; }

	const T& operator () (unsigned int x, unsigned int y) const
	{
		if (y * m_width + x < m_bufferSize)
		{
			return m_buffer[y * m_width + x];
		}
		else
		{
			std::cout << "Tried to access element " << x << "-" << y << " of a " << m_width << "-" << m_height << " array" << std::endl;
			return m_buffer[0];
		}
	}

	T& operator () (unsigned int x, unsigned int y)
	{
		if (y * m_width + x < m_bufferSize)
		{
			return m_buffer[y * m_width + x];
		}
		else
		{
			std::cout << "Tried to access element " << x << "-" << y << " of a " << m_width << "-" << m_height << " array" << std::endl;
			return m_buffer[0];
		}
	}

	void operator = (const Array2D<T>& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;
		m_bufferSize = other.m_bufferSize;

		if (m_buffer != nullptr)
		{
			delete[] m_buffer;
		}

		m_buffer = new T[m_bufferSize];

		for (unsigned int x = 0; x < m_width; x++)
		{
			for (unsigned int y = 0; y < m_height; y++)
			{
				m_buffer[y * m_width + x] = other.m_buffer[y * m_width + x];
			}
		}
	}

	const bool operator == (Array2D<T> other)
	{
		if (m_bufferSize != other.m_bufferSize)
		{
			return false;
		}

		for (unsigned int x = 0; x < m_width; x++)
		{
			for (unsigned int y = 0; y < m_height; y++)
			{
				if (m_buffer[y * m_width + x] != other.m_buffer[y * m_width + x])
				{
					return false;
				}
			}
		}

		return true;
	}

private:

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bufferSize;

	T* m_buffer = nullptr;
};