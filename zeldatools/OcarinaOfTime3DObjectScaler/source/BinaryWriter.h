#pragma once
#include <string>
#include <fstream>

class BinaryWriter
{
public:

	BinaryWriter() {};
	BinaryWriter(const std::string& filename);
	~BinaryWriter();

	template<class T>
	void write(T data);

	void seek(int offset, bool relative);

private:

	std::ofstream m_file;
};

template<class T>
inline void BinaryWriter::write(T data)
{
	m_file.write((char*)&data, sizeof(data));
}
