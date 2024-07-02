#pragma once

#include <iostream>
#include <cstdint>

struct CommonParameter {
    const char* name;
    const char* assembly;
    const uint32_t instruction;
};

inline std::ostream& operator << (std::ostream& out, const CommonParameter& v) {
    out << v.name;
    out << " " << std::hex << v.instruction << std::dec;
    out << " " << v.assembly;
    return out;
}
