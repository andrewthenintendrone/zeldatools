#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include "endian.h"

class BinaryReader
{

public:

	BinaryReader() {};
	BinaryReader(const std::string& filename);
	~BinaryReader();

	void open(const std::string& filename);
	void seek(std::streamoff position, bool relative = false);

	template<typename T>
	inline T read(bool swapEndian = false)
	{
		T data;
		m_file.read(reinterpret_cast<char*>(&data), sizeof(data));

		return swapEndian ? swap_endian(data) : data;
	}

	std::string readNullTerminatedString();

	std::streampos getFileSize() const { return m_filesize; }
	std::streampos getFileOffset() { return m_file.tellg(); }

private:

	std::ifstream m_file;

	std::streampos m_filesize;

	std::string m_filename;
};