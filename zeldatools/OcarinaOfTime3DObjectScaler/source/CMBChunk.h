#pragma once
#include <cstdint>

class CMBChunk
{
public:

	char m_magic[4];
	uint32_t m_sizeOfChunk;
	uint32_t m_numberOfContainedChunks;
	uint32_t m_unknown;
	char m_nameOfChunk[16];
	uint32_t m_numberOfVertexIndices;
	uint32_t m_SKLChunkPointer;
	uint32_t m_MATSChunkPointer;
	uint32_t m_TEXChunkPointer;
	uint32_t m_SKLMChunkPointer;
	uint32_t m_LUTSChunkPointer;
	uint32_t m_VATRChunkPointer;
	uint32_t m_vertexIndicesDataPointer;
	uint32_t m_textureDataPointer;
};