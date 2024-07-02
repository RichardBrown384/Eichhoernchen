#pragma once

#include <cstdint>

#include "Common/BitUtil.h"

// Common properties

constexpr auto InstructionConditionCode(uint32_t v) {
    return (v >> 28u) & 0xFu;
}

constexpr auto InstructionType(uint32_t v) {
    return (v >> 25u) & 0x7u;
}

constexpr auto InstructionRegisterRn(uint32_t v) {
    return (v >> 16u) & 0xFu;
}

constexpr auto InstructionRegisterRd(uint32_t v) {
    return (v >> 12u) & 0xFu;
}

// Data processing opcode

constexpr auto InstructionDataProcessingOpcode(uint32_t v) {
    return (v >> 21u) & 0xFu;
}

constexpr auto InstructionDataProcessingPsrBit(uint32_t v) {
    return BitSet(v, 20u);
}

constexpr auto InstructionDataProcessingRegisterShiftInvalidBit(uint32_t v) {
    return BitSet(v, 7u);
}

constexpr auto InstructionDataProcessingRegisterShiftBit(uint32_t v) {
    return BitSet(v, 4u);
}

// Operand 2
// | Rs | 0 | Type | 1  | Rm |
// | Shift  | Type | 0  | Rm |

constexpr auto InstructionRegisterRs(uint32_t v) {
    return (v >> 8u) & 0xFu;
}

constexpr auto InstructionShiftAmount(uint32_t v) {
    return (v >> 7u) & 0x1Fu;
}

constexpr auto InstructionShiftType(uint32_t v) {
    return (v >> 5u) & 0x3u;
}

constexpr auto InstructionRegisterRm(uint32_t v) {
    return v & 0xFu;
}

// Operand 2
// | Rotate | Immediate |

constexpr auto InstructionRotateAmount(uint32_t v) {
    return (v >> 8u) & 0xFu;
}

constexpr auto InstructionImmediate(uint32_t v) {
    return v & 0xFFu;
}

// Single data transfer

constexpr auto InstructionSingleDataTransferOffset(uint32_t v) {
    return v & 0xFFFu;
}

constexpr auto InstructionSingleDataTransferPreIndexBit(uint32_t v) {
    return BitSet(v, 24u);
}

constexpr auto InstructionSingleDataTransferUpBit(uint32_t v) {
    return BitSet(v, 23u);
}

constexpr auto InstructionSingleDataTransferByteBit(uint32_t v) {
    return BitSet(v, 22u);
}

constexpr auto InstructionSingleDataTransferWriteBackBit(uint32_t v) {
    return BitSet(v, 21u);
}

constexpr auto InstructionSingleDataTransferLoadBit(uint32_t v) {
    return BitSet(v, 20u);
}

constexpr auto InstructionSingleDataTransferRegisterInvalidBit(uint32_t v) {
    return BitSet(v, 4u);
}

// Block data transfer

constexpr auto InstructionBlockDataTransferPsrBit(uint32_t v) {
    return BitSet(v, 22u);
}

constexpr auto InstructionBlockDataTransferRegisterList(uint32_t v) {
    return v & 0xFFFFu;
}

// Branch offset

constexpr auto InstructionBranchLinkBit(uint32_t v) {
    return BitSet(v, 24u);
}

constexpr auto InstructionBranchOffset(uint32_t v) {
    return v & 0xFFFFFFu;
}

// Coprocessor fields

constexpr auto InstructionCoprocessorSwiBit(uint32_t v) {
    return BitSet(v, 24u);
}

constexpr auto InstructionCoprocessorRegisterTransferLoadBit(uint32_t v) {
    return BitSet(v, 20u);
}

constexpr auto InstructionCoprocessorRegisterTransferOpcode(uint32_t v) {
    return (v >> 21u) & 0x7u;
}

constexpr auto InstructionCoprocessorRegisterTransferBit(uint32_t v) {
    return BitSet(v, 4u);
}

constexpr auto InstructionCoprocessorDataTransferPreIndexBit(uint32_t v) {
    return BitSet(v, 24u);
}

constexpr auto InstructionCoprocessorDataTransferUpBit(uint32_t v) {
    return BitSet(v, 23u);
}

constexpr auto InstructionCoprocessorDataTransferLengthBit(uint32_t v) {
    return BitSet(v, 22u);
}

constexpr auto InstructionCoprocessorDataTransferWriteBackBit(uint32_t v) {
    return BitSet(v, 21u);
}

constexpr auto InstructionCoprocessorDataTransferLoadBit(uint32_t v) {
    return BitSet(v, 20u);
}

constexpr auto InstructionCoprocessorDataOperationOpcode(uint32_t v) {
    return (v >> 20u) & 0xFu;
}

constexpr auto InstructionCoprocessorRegisterCn(uint32_t v) {
    return (v >> 16u) & 0xFu;
}

constexpr auto InstructionCoprocessorRegisterCd(uint32_t v) {
    return (v >> 12u) & 0xFu;
}

constexpr auto InstructionCoprocessorNumber(uint32_t v) {
    return (v >> 8u) & 0xFu;
}

constexpr auto InstructionCoprocessorInfo(uint32_t v) {
    return (v >> 5u) & 0x7u;
}

constexpr auto InstructionCoprocessorRegisterCm(uint32_t v) {
    return v & 0xFu;
}

constexpr auto InstructionCoprocessorDataTransferOffset(uint32_t v) {
    return v & 0xFFu;
}

// Software interrupt

constexpr auto InstructionSoftwareInterruptComment(uint32_t v) {
    return v & 0xFFFFFFu;
}
