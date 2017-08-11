#include <cstdint>

struct Tile
{
    uint16_t m_heights[256][256]{ 0 };
    float centerX = 0.0f;
    float centerY = 0.0f;
};