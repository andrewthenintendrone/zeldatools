#pragma once
#include <cstdint>

struct Tile
{
    uint32_t centerX;
    uint32_t centerY;
    uint32_t edgeLength;
    float unk0;
    float unk1;
    float unk2;
    float unk3;
    uint32_t unk4;
    uint32_t stringOffset;
    uint32_t unk5;
    uint32_t unk6;
    uint32_t unk7;

    char tileName[11];
};