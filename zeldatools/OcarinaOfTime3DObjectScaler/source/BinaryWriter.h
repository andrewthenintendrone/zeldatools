#pragma once
#include <string>
#include <fstream>

class BinaryWriter
{
public:

	BinaryWriter() {};
	BinaryWriter(std::string& filename);
	~BinaryWriter();

	template<class T>
	void write(T data);

private:

	std::ofstream m_file;
};

template<class T>
inline void BinaryWriter::write(T data)
{
	m_file.write(reinterpret_cast<char*>(&data), sizeof(data));
}
