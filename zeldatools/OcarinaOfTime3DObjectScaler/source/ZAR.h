#pragma once
#include <cstdint>

class ZARHeader
{
public:

	char m_magic[4];
	uint32_t m_sizeOfArchive;
	uint16_t m_types;
	uint16_t m_numberOfFiles;
	uint32_t m_offsetToFiletypesSection;
	uint32_t m_offsetToFileMetadataSection;
	uint32_t m_offsetToDataSection;
	char m_queen[8];
};

class ZARFiletypesSection
{
public:

	uint32_t m_numFiles;
	uint32_t m_offsetToFilesList;
	uint32_t m_offsetToTypeName;
	uint32_t m_unknown;
};

class ZARFileMetadatSection
{
	uint32_t m_filesize;
	uint32_t m_offsetToFilename;
};