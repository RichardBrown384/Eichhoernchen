#pragma once

#include <cstdint>

constexpr auto SignExtend24(uint32_t n) {
    return ((n & 0xFFFFFFu) ^ 0x800000u) - 0x800000u;
}