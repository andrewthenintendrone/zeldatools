#pragma once
#include <fstream>
#include <string>

class BinaryReader
{
public:

	BinaryReader() {};
	BinaryReader(const std::string& filename);
	~BinaryReader();

	void loadFile(const std::string& filename);

	template<class T>
	T read();

	template<class T>
	T readFromOffset(int offset, bool relative);

	std::string readNullTerminatedString();

	void seek(int offset, bool relative);

	const std::streamoff filesize() { return m_filesize; }

	std::streamoff currentOffset() { return m_file.tellg(); }

private:

	std::string m_filename;
	std::streamoff m_filesize;
	std::ifstream m_file;
};

template<class T>
inline T BinaryReader::read()
{
	T data;
	m_file.read((char*)&data, sizeof(data));
	return data;
}

template<class T>
inline T BinaryReader::readFromOffset(int offset, bool relative)
{
	// store current position in file before seeking to offset
	std::streamoff currentPosition = m_file.tellg();
	seek(offset, relative);

	// read data
	T data = read<T>();

	// seek back to stored position
	m_file.seekg(currentPosition, std::ios::beg);

	return data;
}
