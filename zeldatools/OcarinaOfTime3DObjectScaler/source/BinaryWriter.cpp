#include "BinaryWriter.h"
#include <iostream>

BinaryWriter::BinaryWriter(const std::string& filename)
{
	m_file.open(filename, std::ios::binary | std::ios::trunc);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
	}
}

BinaryWriter::~BinaryWriter()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}

void BinaryWriter::seek(int offset, bool relative)
{
	int way = relative ? std::ios::cur : std::ios::beg;
	m_file.seekp(offset, way);
}
