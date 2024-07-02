#include <cstdint>

#include "Common/BitUtil.h"
#include "Common/Decode.h"
#include "Common/Constants.h"
#include "Common/Shifts.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

auto Arm::ExecuteDataProcessingRegisterShift(uint32_t instruction) -> void {
    const auto shiftInvalidBit = InstructionDataProcessingRegisterShiftInvalidBit(instruction);
    const auto opcode = InstructionDataProcessingOpcode(instruction);
    if (shiftInvalidBit) {
        switch (opcode) {
            case DATA_OPCODE_AND: return Mul(instruction, ReadOperandsMultiplyAccumulate(instruction));
            case DATA_OPCODE_EOR: return Mla(instruction, ReadOperandsMultiplyAccumulate(instruction));
                [[unlikely]] default: return;
        }
    }
    const auto& operands = ReadOperandsRegisterShift(instruction);
    switch (opcode) {
        case DATA_OPCODE_AND: return And(instruction, operands);
        case DATA_OPCODE_EOR: return Eor(instruction, operands);
        case DATA_OPCODE_SUB: return Sub(instruction, operands);
        case DATA_OPCODE_RSB: return Rsb(instruction, operands);
        case DATA_OPCODE_ADD: return Add(instruction, operands);
        case DATA_OPCODE_ADC: return Adc(instruction, operands);
        case DATA_OPCODE_SBC: return Sbc(instruction, operands);
        case DATA_OPCODE_RSC: return Rsc(instruction, operands);
        case DATA_OPCODE_TST: return Tst(instruction, operands);
        case DATA_OPCODE_TEQ: return Teq(instruction, operands);
        case DATA_OPCODE_CMP: return Cmp(instruction, operands);
        case DATA_OPCODE_CMN: return Cmn(instruction, operands);
        case DATA_OPCODE_ORR: return Orr(instruction, operands);
        case DATA_OPCODE_MOV: return Mov(instruction, operands);
        case DATA_OPCODE_BIC: return Bic(instruction, operands);
        case DATA_OPCODE_MVN: return Mvn(instruction, operands);
            [[unlikely]] default: return;
    }
}

constexpr auto MultiplyAccumulateInternalCycleCount(uint32_t n) {
    const auto bitWidth = 32u - CountLeadingZeros32(n);
    return std::min(1u + LogicalShiftRight(bitWidth, 1u), 16u);
}

auto Arm::ReadOperandsMultiplyAccumulate(uint32_t instruction) -> MultiplyAccumulateOperands {
    const auto mode = GetMode();
    const auto d = InstructionRegisterRn(instruction);
    const auto n = InstructionRegisterRd(instruction);
    const auto s = InstructionRegisterRs(instruction);
    const auto m = InstructionRegisterRm(instruction);
    const auto rs = GetRegisterWithoutPSR(mode, s);
    const auto rn = GetRegisterWithPSR(mode, n);
    CycleS();
    PrefetchInstruction();
    IncrementPC();
    const auto rm = GetRegisterWithPSR(mode, m);
    CycleI(MultiplyAccumulateInternalCycleCount(rs));
    return {
        .op1 = (d == m) ? 0u : rm,
        .op2 = (d == m) ? 0u : rs,
        .accumulator = rn
    };
}

auto Arm::ReadOperandsRegisterShift(uint32_t instruction) -> DataProcessingOperands {
    const auto mode = GetMode();
    const auto s = InstructionRegisterRs(instruction);
    const auto m = InstructionRegisterRm(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto rs = GetRegisterWithoutPSR(mode, s) & 0xFFu;
    CycleS();
    PrefetchInstruction();
    IncrementPC();
    const auto rm = GetRegisterWithPSR(mode, m);
    const auto rn = GetRegisterWithoutPSR(mode, n);
    const auto op1 = rn;
    uint32_t op2, carry;
    ReadOperand2RegisterShift(instruction, rm, rs, op2, carry);
    CycleS();
    return {
        .op1 = op1,
        .op2 = op2,
        .carry = carry
    };
}

void Arm::ReadOperand2RegisterShift(
    uint32_t instruction,
    uint32_t m,
    uint32_t s,
    uint32_t& op2,
    uint32_t& carry) const {
    switch (InstructionShiftType(instruction)) {
        case SHIFT_TYPE_LSL: {
            if (!s) {
                op2 = m;
                carry = LogicalShiftLeft(GetC(), 31u);
            } else if (s < 32u) {
                op2 = LogicalShiftLeft(m, s);
                carry = LogicalShiftLeft(m, s - 1u);
            } else if (s == 32u) {
                op2 = 0;
                carry = LogicalShiftLeft(m, 31u);
            } else {
                op2 = carry = 0u;
            }
            return;
        }
        case SHIFT_TYPE_LSR: {
            if (!s) {
                op2 = m;
                carry = LogicalShiftLeft(GetC(), 31u);
            } else if (s < 32u) {
                op2 = LogicalShiftRight(m, s);
                carry = LogicalShiftLeft(m, 32u - s);
            } else if (s == 32u) {
                op2 = 0u;
                carry = m;
            } else {
                op2 = carry = 0u;
            }
            return;
        }
        case SHIFT_TYPE_ASR: {
            if (!s) {
                op2 = m;
                carry = LogicalShiftLeft(GetC(), 31u);
            } else if (s < 32u) {
                op2 = ArithmeticShiftRight(m, s);
                carry = LogicalShiftLeft(m, 32u - s);
            } else {
                op2 = ArithmeticShiftRight(m, 31u);
                carry = m;
            }
            return;
        }
        case SHIFT_TYPE_ROR: {
            if (!s) {
                op2 = m;
                carry = LogicalShiftLeft(GetC(), 31u);
            } else {
                const uint32_t t = s & 0x1Fu;
                op2 = RotateRight(m, t);
                carry = LogicalShiftLeft(m, 32u - t);
            }
            return;
        }
        default: {
            op2 = 0;
            carry = 0;
            return;
        }
    }
}