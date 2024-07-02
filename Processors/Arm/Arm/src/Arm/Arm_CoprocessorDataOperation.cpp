#include <cstdint>

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteCoprocessorDataOperation(uint32_t) -> void {
    ExceptionUndefinedInstruction();
}
