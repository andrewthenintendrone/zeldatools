#pragma once
#include <cstdint>

struct TSCBHeader
{
    char magic[5] = "    ";
    uint32_t unknown = 0;
    uint32_t unknown2 = 0;
    uint32_t stringTableOffset = 0;
    float unknownFloat = 0.0f;
    float unknownFloat2 = 0.0f;
    uint32_t materialLookupTableCount = 0;
    uint32_t tileTableCount = 0;
    uint32_t unknown3 = 0;
    uint32_t unknown4 = 0;
    float unknownFloat3 = 0.0f;
    uint32_t unknown5 = 0;
    uint32_t sizeOfMaterialLookupTable = 0;
};