#include <cstdint>

#include "Common/BitUtil.h"
#include "Common/Constants.h"
#include "Common/Decode.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

/*
 * VLSI Manual Cycle Summary for LDM
 *
 * The first cycle of is used to calculate the address of the first word
 * to be transferred while performing a prefetch from memory.
 *
 * The cycle table says that PC + 8 is on the address bus.
 * However, when R15 is being used as the base address the manual, as far
 * as I can tell doesn't say if PC + 8 or PC + 12 should be used.
 *
 * We assume that PC + 8 is used.
 *
 * The second cycle fetches the first value from memory and performs
 * the base register modification
 *
 * During the third cycle the first word is moved into the appropriate
 * destination register, while the second word is fetched from memory,
 * and the modified base is latched in the ALU A Bus for patching up after an abort.
 *
 * The third cycle is repeated as many times as necessary.
 *
 * The final internal cycle moves the last retrieved word to the destination
 * register.
 *
 * If an abort occurs the final cycle is modified to write back to
 * the base register again (I presume using with the same value in the second cycle).
 * This is done in case the base register was overwritten during the load.
 * E.g. LDM R5!, {R1,R2,R5,R6,R8,R9} when an exception occurs while attempting to load R9.
 *
 * If an abort occurs the instruction continues to completion but with all
 * further register writes prohibited.
 *
 * VLSI Manual General Notes For LDM
 *
 * R15 as the Base - When the base is the PC, the PSR bits will be
 * used to form the address as well so unless all interrupts are enabled and
 * all flags are zero an address exception will occur.
 * Also, write back is never allowed with the base is the PC (setting the W bit will
 * have no effect).
 *
 * Transfers to User Bank
 *
 * The documentation for LDM doesn't say very much about the specifics about write back in comparison to
 * STM. It kind of implies that the behaviour is the same as STM. However, the Floating Point Emulator
 * shipped with RISC OS uses the following stack manipulation code on entry and exit.
 *
 * SUB R13, R13, #64        ; Subtract 64 from the supervisor stack pointer (R13)
 * STMIA R13, {R0-R14}^     ; Push all the user registers onto the stack (60 bytes)
 * STR R14, [R13, #60]      ; Push the exception program counter (R14) onto the stack
 * ...
 * LDMIA R13!, {R0-R14}^    ; Pop all the user registers back off the stack, update stack pointer
 * LDMIA R13!, {R15}^       ; Pop the exception program counter back off the stack and update the stack pointer
 *
 * In order for this code to work the supervisor's stack pointer has to be updated. Not the user's!
 *
 * Address exceptions
 *
 * The documentation says the state is the same as if a data abort had occurred
 * during the first transfer cycle. One assumes this means the registers are unchanged
 * except the base if write-back had been specified.
 *
 */

auto Arm::ExecuteBlockDataTransferLoad(uint32_t instruction) -> void {
    // LDM first cycle
    // 1. Calculate the first transfer address (when R15 is used, PSR bits are presented)
    // 2. Prefetch is performed (S cycle)

    const auto preIndexedBit = InstructionSingleDataTransferPreIndexBit(instruction);
    const auto upBit = InstructionSingleDataTransferUpBit(instruction);
    const auto psrBit = InstructionBlockDataTransferPsrBit(instruction);
    const auto writeBackBit = InstructionSingleDataTransferWriteBackBit(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto list = InstructionBlockDataTransferRegisterList(instruction);
    const auto r15InList = BitSet(list, 15u);

    const auto base = GetRegisterWithPSR(GetMode(), n);
    const auto count = CountBits16(list);

    const auto byteLength = 4u * count;
    const auto writeBackAddress = base + byteLength * (2u * upBit - 1u);
    auto transferAddress = base - byteLength * !upBit + 4u * !(upBit ^ preIndexedBit);

    const auto transferBank = (psrBit && !r15InList) ? PSR_MODE_USR : GetMode();
    const auto first = CountTrailingZeros16(list);

    CycleS();
    PrefetchInstruction();
    IncrementPC();

    // In the event of an address exception the VLSI manual the instruction
    // completes as normal but register writes are prevented. Handle as a special case:
    if (TransferAddressIsInvalid(transferAddress)) {
        // LDM second cycle
        uint32_t discard;
        if (first < 16u) {
            CycleN();
            ReadWord(transferAddress, discard);
        }
        if (writeBackBit && !IsPC(n)) {
            SetRegister(GetMode(), n, writeBackAddress);
        }
        // Subsequent cycles
        for (auto i = first + 1; i < 16u; ++i) {
            if (BitSet(list, i)) {
                transferAddress += 4;
                CycleS();
                ReadWord(transferAddress, discard);
            }
        }
        // Final internal cycle (used to fix up base register, which isn't required in this case
        // since writes were prevented)
        CycleI();
        ExceptionAddressException();
        return;
    }

    // LDM second cycle
    // 1. Fetches the first word
    // 2. Performs base register modification
    CycleN();
    uint32_t value;
    auto dataAbort = !ReadWord(transferAddress, value);

    if (writeBackBit && !IsPC(n)) {
        SetRegister(GetMode(), n, writeBackAddress);
    }

    // LDM Third cycle we start sequential fetching
    auto previous = first;
    for (auto i = first + 1u; i < 16u; ++i) {
        if (BitSet(list, i)) {
            uint32_t nextValue;
            transferAddress += 4u;
            CycleS();
            dataAbort |= !ReadWord(transferAddress, nextValue);
            if (!dataAbort) {
                SetRegister(transferBank, previous, value);
            }
            value = nextValue;
            previous = i;
        }
    }

    // Handle data abort
    if (dataAbort) {
        // Final internal cycle, fix up base register
        CycleI();
        if (writeBackBit && !IsPC(n)) {
            SetRegister(GetMode(), n, writeBackAddress);
        }
        ExceptionDataAbort();
        return;
    }

    // Final internal cycle to update final register
    CycleI();
    if (IsPC(previous)) {
        if (psrBit) {
            DeconstructPSR(value);
        }
        Branch(value);
    } else if (previous < 15u) {
        SetRegister(transferBank, previous, value);
    }
}