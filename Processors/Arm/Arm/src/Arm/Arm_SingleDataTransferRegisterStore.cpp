#include <cstdint>

#include "Common/Decode.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteSingleDataTransferRegisterStore(uint32_t instruction) -> void {
    // STR first Cycle
    // The first cycle performs the address calculation
    // And one presumes the prefetch

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto upBit = InstructionSingleDataTransferUpBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto s = InstructionShiftAmount(instruction);
    const auto m = InstructionRegisterRm(instruction);

    const auto rn = GetRegisterWithoutPSR(GetMode(), n);
    const auto rm = GetRegisterWithPSR(GetMode(), m);

    uint32_t op2, carry;
    ReadOperand2InstructionShift(instruction, rm, s, op2, carry);
    const auto transferAddress = rn + preIndexedBit * op2 * (2u * upBit - 1u);
    const auto writeBackAddress = rn + op2 * (2u * upBit - 1u);

    CycleN();
    PrefetchInstruction();
    IncrementPC();

    ExecuteSingleDataTransferStore(instruction, transferAddress, writeBackAddress);
}
