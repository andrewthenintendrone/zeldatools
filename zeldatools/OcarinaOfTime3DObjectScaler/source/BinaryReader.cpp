#include "BinaryReader.h"
#include <iostream>

BinaryReader::BinaryReader(const std::string& filename)
{
	loadFile(filename);
}

BinaryReader::~BinaryReader()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}

void BinaryReader::loadFile(const std::string& filename)
{
	m_filename = filename;

	m_file.open(filename, std::ios::binary | std::ios::_Nocreate | std::ios::ate);

	if (!m_file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
	}

	m_filesize = m_file.tellg();

	m_file.seekg(0, std::ios::beg);
}

std::string BinaryReader::readNullTerminatedString()
{
	std::string str;

	while (true)
	{
		char currentChar = read<char>();

		if (currentChar == '\0')
		{
			break;
		}

		str += currentChar;
	}

	return str;
}

void BinaryReader::seek(int offset, bool relative)
{
	int way = relative ? std::ios::cur : std::ios::beg;
	m_file.seekg(offset, way);
}
