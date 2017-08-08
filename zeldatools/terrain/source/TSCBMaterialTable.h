#pragma once
#include <cstdint>

struct TSCBMaterialTable
{
    uint32_t offset = 0;
    uint32_t index = 0;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    float alpha = 0.0f;
};