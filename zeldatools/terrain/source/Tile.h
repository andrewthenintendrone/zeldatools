#pragma once
#include <cstdint>

struct Tile
{
    uint16_t m_heights[256][256]{ 0 };
    float m_xOffset = 0.0f;
    float m_yOffset = 0.0f;
};