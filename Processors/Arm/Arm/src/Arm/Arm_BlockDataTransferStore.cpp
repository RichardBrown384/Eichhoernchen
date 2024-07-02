#include <cstdint>

#include "Common/BitUtil.h"
#include "Common/Decode.h"
#include "Common/Constants.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

/*
 *  VLSI Manual Cycle Summary for STM
 *
 * The first cycle of is used to calculate the address of the first word
 * to be transferred while performing a prefetch from memory.
 *
 * The cycle table says that PC + 8 is on the address bus.
 * However, when R15 is being used as the base address the manual, as far
 * as I can tell doesn't say if PC + 8 or PC + 12 should be used.
 *
 * The second cycle writes the first value to memory and performs
 * the base register modification. In that order.
 *
 * VLSI Manual General notes for STM
 *
 * R15 as the Base - When the base is the PC, the PSR bits will be
 * used to form the address as well so unless all interrupts are enabled and
 * all flags are zero an address exception will occur.
 * Also, write back is never allowed with the base is the PC (setting the W bit will
 * have no effect).
 *
 *  Transfers to User Bank
 *  For STM instructions the S bit is redundant as the PSR is always stored with
 *  the PC whenever R15 is in the transfer list.
 *
 *  In user mode the S bit is ignored but in other modes S=1 is used to
 *  force transfers to take values fom the user bank.
 *
 *  Note that write back to the base will also be in the user bank
 *  although the base will be fetched from the current bank.
 *
 *  Aborts during STM
 *  If the data abort occurs, the processor takes little action until
 *  the instruction completes, whereupon it enters the data abort trap.
 *
 *  _The memory manager is responsible for preventing erroneous writes_
 *
 */

auto Arm::ExecuteBlockDataTransferStore(uint32_t instruction) -> void {
    // STM first cycle
    // VLSI documentation implies:
    // 1. Calculate the first transfer address (when R15 is used, PSR bits are presented)
    // 2. Prefetch is performed (S cycle)

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto upBit = InstructionSingleDataTransferUpBit(instruction);
    const auto psrBit = InstructionBlockDataTransferPsrBit(instruction);
    const auto writeBackBit = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto list = InstructionBlockDataTransferRegisterList(instruction);

    const auto base = GetRegisterWithPSR(GetMode(), n);
    const auto count = CountBits16(list);

    const auto byteLength = 4u * count;
    const auto writeBackAddress = base + byteLength * (2u * upBit - 1u);
    auto transferAddress = base - byteLength * !upBit + 4u * !(upBit ^ preIndexedBit);

    const auto transferBank = psrBit ? PSR_MODE_USR : GetMode();
    const auto first = CountTrailingZeros16(list);

    CycleN();
    PrefetchInstruction();
    IncrementPC();

    // In the event of an address exception the VLSI manual
    // says all stores are converted to loads, handle this as a special case
    if (TransferAddressIsInvalid(transferAddress)) {
        // STM second cycle
        uint32_t discard;
        if (first < 16u) {
            CycleN();
            ReadWord(transferAddress, discard);
        }
        if (writeBackBit && !IsPC(n)) {
            SetRegister(transferBank, n, writeBackAddress);
        }
        // Subsequent cycles
        for (auto i = first + 1; i < 16u; ++i) {
            if (BitSet(list, i)) {
                transferAddress += 4;
                CycleS();
                ReadWord(transferAddress, discard);
            }
        }
        ExceptionAddressException();
        return;
    }

    // STM second cycle
    // 1. Writes the first word
    // 2. Performs base register modification
    auto dataAbort = false;
    if (first < 16u) {
        const auto value = GetRegisterWithPSR(transferBank, first);
        CycleN();
        dataAbort |= !WriteWord(transferAddress, value);
    }

    if (writeBackBit && !IsPC(n)) {
        SetRegister(transferBank, n, writeBackAddress);
    }

    // Subsequent cycles
    for (auto i = first + 1u; i < 16u; ++i) {
        if (BitSet(instruction, i)) {
            transferAddress += 4u;
            const auto value = GetRegisterWithPSR(transferBank, i);
            CycleS();
            dataAbort |= !WriteWord(transferAddress, value);
        }
    }

    if (dataAbort) {
        ExceptionDataAbort();
    }

}