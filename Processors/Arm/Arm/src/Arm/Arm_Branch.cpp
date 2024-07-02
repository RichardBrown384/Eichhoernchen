#include <cstdint>

#include "Common/Decode.h"
#include "Common/Constants.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteBranch(uint32_t instruction) -> void {
    CycleS();
    PrefetchInstruction();
    if (InstructionBranchLinkBit(instruction)) {
        const auto v = GetBranchLinkValue();
        SetRegister(GetMode(), REGISTER_R14, v);
    }
    const auto target = GetBranchTarget(instruction);
    Branch(target);
}