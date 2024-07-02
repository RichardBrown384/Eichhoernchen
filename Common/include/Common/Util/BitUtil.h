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

constexpr auto ReplaceBitField(uint32_t v, uint32_t bit, uint32_t width, uint32_t replacement) -> uint32_t {
    const auto mask = ((1u << width) - 1u) << bit;
    return (v & ~mask) + ((replacement << bit) & mask);
}

constexpr auto ReplicateByteInWord(uint32_t value) -> uint32_t {
    return ExtractBitField(value, 0u, 8u) * 0x01'01'01'01u;
}

constexpr auto ReplicateHalfWordInWord(uint32_t value) -> uint32_t {
    return ExtractBitField(value, 0u, 16u) * 0x00'01'00'01u;
}

constexpr auto CountTrailingZeroes8(uint32_t v) -> uint32_t {
    return std::countr_zero(static_cast<uint8_t>(v));
}