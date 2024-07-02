#pragma once

// Note: This file duplicates one of the private headers in the main library
// In future, we should move to a model where there's a shared library of common functions
// across all the assemblers, disassemblers and emulators.

#include <cstdint>

constexpr auto ExtractBitField(uint32_t v, uint32_t bit, uint32_t width) {
    return (v >> bit) & ((1u << width) - 1u);
}

constexpr auto EncodeBitField(uint32_t v, uint32_t bit, uint32_t width) {
    return (v & ((1u << width) - 1u)) << bit;
}