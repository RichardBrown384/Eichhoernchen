#include <cstdint>

#include "Common/Decode.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

/*
 * VLSI Manual Cycle Summary for STR
 *
 * The first cycle performs the address calculation
 *
 * The second cycle writes to memory and performs the base modification
 */

auto Arm::ExecuteSingleDataTransferImmediateStore(uint32_t instruction) -> void {
    // STR first Cycle
    // The first cycle performs the address calculation
    // And one presumes the prefetch

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto upBit = InstructionSingleDataTransferUpBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto offset = InstructionSingleDataTransferOffset(instruction);

    const auto rn = GetRegisterWithoutPSR(GetMode(), n);

    const auto transferAddress = rn + preIndexedBit * offset * (2u * upBit - 1u);
    const auto writeBackAddress = rn + offset * (2u * upBit - 1u);

    CycleN();
    PrefetchInstruction();
    IncrementPC();

    ExecuteSingleDataTransferStore(instruction, transferAddress, writeBackAddress);
}

void Arm::ExecuteSingleDataTransferStore(uint32_t instruction, uint32_t transferAddress, uint32_t writeBackAddress) {

    const auto addressException = TransferAddressIsInvalid(transferAddress);

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto byteBit = InstructionSingleDataTransferByteBit(instruction);
    const auto writeBackBit = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto d = InstructionRegisterRd(instruction);

    auto value = GetRegisterWithPSR(GetMode(), d);

    // STR second cycle
    // 1. Value is written to memory
    // 2. Base register is written back to
    const auto transPin = GetTransPin();
    if (!preIndexedBit && writeBackBit) {
        SetTransPin(false);
    }
    CycleN();
    // Handle address exceptions here, convert stores to loads
    if (addressException) {
        if (byteBit) {
            ReadByte(transferAddress, value);
        } else {
            ReadWordUnaligned(transferAddress, value);
        }
        SetTransPin(transPin);
        ExceptionAddressException();
        return;
    }
    const auto dataAbort = byteBit ?
                           !WriteByte(transferAddress, value) :
                           !WriteWord(transferAddress, value);
    SetTransPin(transPin);

    // If a data abort occurs then prevent base register write-back
    if (dataAbort) {
        ExceptionDataAbort();
        return;
    }

    // In post-indexed mode write-back always takes place
    if (!preIndexedBit || writeBackBit) {
        if (IsPC(n)) {
            Branch(writeBackAddress);
        } else {
            SetRegister(GetMode(), n, writeBackAddress);
        }
    }
}