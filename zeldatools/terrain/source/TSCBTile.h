#pragma once
#include <cstdint>

struct TSCBTile
{
    float centerX = 0.0f;
    float centerY = 0.0f;
    float edgeLength = 0.0f;
    float unk0 = 0.0f;
    float unk1 = 0.0f;
    float unk2 = 0.0f;
    float unk3 = 0.0f;
    uint32_t unk4 = 0;
    uint32_t stringOffset = 0;
    uint32_t unk5 = 0;
    uint32_t unk6 = 0;
    uint32_t unk7 = 0;
    uint32_t extradata = 0;

    char tileName[13] = "            ";
};