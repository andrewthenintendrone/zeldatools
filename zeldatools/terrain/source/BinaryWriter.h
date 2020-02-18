#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include "endian.h"

class BinaryWriter
{

public:

	BinaryWriter() {};
	BinaryWriter(const std::string& filename);
	~BinaryWriter();

	template<typename T>
	inline void write(T data, bool swapEndian = false)
	{
		if (swapEndian)
		{
			data = swapEndian(data);
		}
		m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
	}

private:

	std::ofstream m_file;

	std::string m_filename;
};