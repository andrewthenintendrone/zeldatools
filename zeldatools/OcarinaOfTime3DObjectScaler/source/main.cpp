#include <iostream>
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "ZAR.h"
#include "CMBChunk.h"
#include <vector>

class VertexListData
{
public:

	uint32_t m_dataSize;
	uint32_t m_dataOffset;
};

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		BinaryReader reader(argv[1]);
		//BinaryReader reader("C:\\Users\\Andrew\\Desktop\\ExtractedRomFS\\actor\\zelda_gi_shield_2.zar");

		// first read entire buffer
		char* buffer = new char[reader.filesize()];

		for (int i = 0; i < reader.filesize(); i++)
		{
			buffer[i] = reader.read<char>();
		}

		reader.seek(0, false);

		std::vector<uint32_t> VATRLocations;

		for (int i = 0; i < reader.filesize() - 4; i += 4)
		{
			char magic[4];

			for (int j = 0; j < 4; j++)
			{
				magic[j] = reader.read<char>();
			}

			if (magic[0] == 'v' && magic[1] == 'a' && magic[2] == 't' && magic[3] == 'r')
			{
				VATRLocations.push_back(reader.currentOffset() - 4);
			}
		}

		std::vector<VertexListData> vertexListDatas;

		for (int i = 0; i < VATRLocations.size(); i++)
		{
			reader.seek(VATRLocations[i] + 12, false);

			VertexListData data;

			data.m_dataSize = reader.read<uint32_t>();
			data.m_dataOffset = VATRLocations[i] + reader.read<uint32_t>();

			vertexListDatas.push_back(data);
		}

		std::vector<std::vector<float>> vertexPositions;

		for (int i = 0; i < vertexListDatas.size(); i++)
		{
			reader.seek(vertexListDatas[i].m_dataOffset, false);

			vertexPositions.push_back(std::vector<float>());

			// read vertex positions
			for (int j = 0; j < vertexListDatas[i].m_dataSize / sizeof(float); j++)
			{
				// scaling gets done here
				vertexPositions[i].push_back(reader.read<float>() * 2.0f);
			}
		}

		std::string outputFilename = argv[1] + std::string("_out");

		//std::string outputFilename = "C:\\Users\\Andrew\\Desktop\\ExtractedRomFS\\actor\\zelda_gi_shield_2.zar_out";

		BinaryWriter writer(outputFilename);

		// write exact copy at first
		for (int i = 0; i < reader.filesize(); i++)
		{
			writer.write<char>(buffer[i]);
		}

		// jump to vertex position offsets and rewrite
		for (int i = 0; i < vertexListDatas.size(); i++)
		{
			writer.seek(vertexListDatas[i].m_dataOffset, false);

			for (int j = 0; j < vertexPositions[i].size(); j++)
			{
				writer.write<float>(vertexPositions[i][j]);
			}

			std::cout << "wrote " << vertexPositions[i].size() / 3 << " vertex positions" << std::endl;
		}
	}

	std::cin.get();
	return 0;
}