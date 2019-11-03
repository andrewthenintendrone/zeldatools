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

private:

	std::string m_filename;
	std::streamoff m_filesize;
	std::ifstream m_file;
};

template<class T>
inline T BinaryReader::read()
{
	T data;
	m_file.read(reinterpret_cast<char*>(&data), sizeof(data));
	return data;
}
