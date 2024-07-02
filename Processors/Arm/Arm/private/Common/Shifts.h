#pragma once

#include <cstdint>

// Please see: https://en.wikipedia.org/wiki/Circular_shift for treatment of edge cases

constexpr auto LogicalShiftLeft(uint32_t v, uint32_t n) {
    return v << (n & 0x1Fu);
}

constexpr auto LogicalShiftRight(uint32_t v, uint32_t n) {
    return v >> (n & 0x1Fu);
}

constexpr auto ArithmeticShiftRight(uint32_t v, uint32_t n) {
    return LogicalShiftRight(v ^ 0x80000000u, n) - LogicalShiftRight(0x80000000u, n);
}

constexpr auto RotateRight(uint32_t value, uint32_t n) {
    return LogicalShiftRight(value, n) | LogicalShiftLeft(value, -(n & 0x1Fu));
}

constexpr auto RotateLeft(uint32_t value, uint32_t n) {
    return LogicalShiftLeft(value, n) | LogicalShiftRight(value, -(n & 0x1Fu));
}
