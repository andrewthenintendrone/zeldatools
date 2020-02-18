#include "BinaryWriter.h"
#include <iostream>

BinaryWriter::BinaryWriter(const std::string& filename)
{
	m_filename = filename;

	m_file.open(m_filename, std::ios::trunc | std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << m_filename << std::endl;
		return;
	}
}

BinaryWriter::~BinaryWriter()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}