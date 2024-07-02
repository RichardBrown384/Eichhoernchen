#include <cstdint>

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteCoprocessorRegisterTransfer(uint32_t) -> void {
    ExceptionUndefinedInstruction();
}
