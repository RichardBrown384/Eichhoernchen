#pragma once

#include <cstdint>
#include <bit>

constexpr auto BitSet(uint32_t value, uint32_t bit) -> bool {
    return (value >> bit) & 0x1u;
}

constexpr auto ExtractBitField(uint32_t v, uint32_t bit, uint32_t width) -> uint32_t {
    return (v >> bit) & ((1u << width) - 1u);
}

constexpr auto EncodeBitField(uint32_t v, uint32_t bit, uint32_t width) -> uint32_t {
    return (v & ((1 << width) - 1u)) << bit;
}

constexpr auto ReplicateByteInWord(uint32_t value) -> uint32_t {
    return ExtractBitField(value, 0u, 8u) * 0x01'01'01'01u;
}

constexpr auto CountBits16(uint32_t value) -> uint32_t {
    return std::popcount(value & 0xFFFFu);
}

constexpr auto CountTrailingZeros16(uint32_t value) -> uint32_t {
    return std::countr_zero(value & 0xFFFFu);
}

constexpr auto CountLeadingZeros32(uint32_t value) -> uint32_t {
    return std::countl_zero(value);
}

