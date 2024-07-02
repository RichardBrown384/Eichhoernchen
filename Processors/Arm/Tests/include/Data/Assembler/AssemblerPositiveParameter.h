#pragma once

#include <iostream>
#include <cstdint>

struct AssemblerPositiveParameter {
    const char* name;
    const char* assembly;
    const uint32_t instruction;
};

inline std::ostream& operator << (std::ostream& out, const AssemblerPositiveParameter& v) {
    out << v.name;
    out << " " << std::hex << v.instruction << std::dec;
    out << " " << v.assembly;
    return out;
}
