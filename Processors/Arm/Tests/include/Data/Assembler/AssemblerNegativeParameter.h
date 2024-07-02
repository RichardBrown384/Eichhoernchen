#pragma once

#include <iostream>
#include <cstdint>

struct AssemblerNegativeParameter {
    const char* name;
    const char* assembly;
};

std::ostream& operator << (std::ostream& out, const AssemblerNegativeParameter& v) {
    out << v.name;
    out << " " << v.assembly;
    return out;
}