#include "BinaryReader.h"
#include <iostream>

BinaryReader::BinaryReader(const std::string& filename)
{
	open(filename);
}

BinaryReader::~BinaryReader()
{
	// close file if it's open
	if (m_file.is_open())
	{
		m_file.close();
	}
}

// open file for reading
void BinaryReader::open(const std::string& filename)
{
	m_filename = filename;

	m_file.open(m_filename, std::ios::_Nocreate | std::ios::ate | std::ios::binary);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << m_filename << std::endl;
		return;
	}

	m_filesize = m_file.tellg();

	// jump back to beginning
	m_file.seekg(0, std::ios::beg);
}

void BinaryReader::seek(std::streamoff position, bool relative)
{
	m_file.seekg(position, relative ? std::ios::cur : std::ios::beg);
}

std::string BinaryReader::readNullTerminatedString()
{
	std::string s;
	char c = 'a';

	while (c != '\0')
	{
		c = read<char>();

		// null terminator reached
		if (c == '\0')
			break;

		s += c;
	}

	return s;
}