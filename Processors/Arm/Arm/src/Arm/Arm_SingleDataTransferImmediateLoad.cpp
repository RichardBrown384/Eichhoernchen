#include <cstdint>

#include "Common/Decode.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

/*
 * VLSI Manual Cycle Summary for LDR
 *
 * The first cycle performs the address calculation
 *
 * The second cycle loads the value from memory and if required updates the base register
 *
 * The third cycle transfers the value to the destination register
 *
 * VLSI Manual General Notes For LDR
 *
 * Use of R15
 *
 * Using R15 as the Rn or Rd will never modify the PSR bits
 *
 * If R15 is used as Rn, the PSR bits are not present, but the PC is eight bytes ahead.
 *
 */

auto Arm::ExecuteSingleDataTransferImmediateLoad(uint32_t instruction) -> void {

    // LDR first Cycle
    // The first cycle performs the address calculation
    // And one presumes the prefetch

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto upBit = InstructionSingleDataTransferUpBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto offset = InstructionSingleDataTransferOffset(instruction);

    const auto rn = GetRegisterWithoutPSR(GetMode(), n);

    const auto transferAddress = rn + preIndexedBit * offset * (2u * upBit - 1u);
    const auto writeBackAddress = rn + offset * (2u * upBit - 1u);

    CycleS();
    PrefetchInstruction();
    IncrementPC();

    ExecuteSingleDataTransferLoad(instruction, transferAddress, writeBackAddress);
}

void Arm::ExecuteSingleDataTransferLoad(uint32_t instruction, uint32_t transferAddress, uint32_t writeBackAddress) {

    const auto addressException = TransferAddressIsInvalid(transferAddress);

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto byteBit = InstructionSingleDataTransferByteBit(instruction);
    const auto writeBackBit = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto d = InstructionRegisterRd(instruction);

    // LDR second cycle
    // 1. Value is fetched from memory
    // 2. Base register is written back to
    const auto transPin = GetTransPin();
    if (!preIndexedBit && writeBackBit) {
        SetTransPin(false);
    }
    CycleN();
    uint32_t value;
    const auto dataAbort = byteBit ?
                           !ReadByte(transferAddress, value) :
                           !ReadWordUnaligned(transferAddress, value);
    SetTransPin(transPin);

    // If there was an address exception then prevent base register write back
    if (addressException) {
        CycleI();
        ExceptionAddressException();
        return;
    }

    // If a data abort occurs then prevent base register write-back
    if (dataAbort) {
        CycleI();
        ExceptionDataAbort();
        return;
    }

    // In post-indexed mode write-back always takes place
    if (!preIndexedBit || writeBackBit) {
        // Potentially skip redundant work if n == d
        if (n != d) {
            // Writing back to the PC causes a branch and since d is not the PC
            // we perform the writes out of order but the result is the same
            if (IsPC(n)) {
                CycleI();
                SetRegister(GetMode(), d, value);
                Branch(writeBackAddress);
                return;
            }
            SetRegister(GetMode(), n, writeBackAddress);
        }
    }

    // Final internal cycle
    // Move value to register
    CycleI();
    if (IsPC(d)) {
        Branch(value);
    } else {
        SetRegister(GetMode(), d, value);
    }
}
