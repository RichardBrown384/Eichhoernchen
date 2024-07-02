#include "Arm/Assembler.h"
#include "Assembler/Assembler_Impl.h"

using namespace rbrown::arm;

bool rbrown::arm::Assemble(const std::string& assembly, uint32_t& result) {
    return Assembler::Assemble(assembly, result);
}