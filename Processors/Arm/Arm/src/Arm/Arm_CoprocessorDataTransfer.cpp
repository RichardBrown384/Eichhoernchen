#include <cstdint>

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteCoprocessorDataTransfer(uint32_t) -> void {
    ExceptionUndefinedInstruction();
}
