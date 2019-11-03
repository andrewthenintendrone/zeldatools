#include "BinaryWriter.h"
#include <iostream>

BinaryWriter::BinaryWriter(std::string& filename)
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
