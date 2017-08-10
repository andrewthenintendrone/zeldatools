#pragma once

template <typename T>
T swap_endian(T objp)
{
    unsigned char *memp = reinterpret_cast<unsigned char*>(&objp);
    std::reverse(memp, memp + sizeof(T));
    return objp;
}