#include <cstdint>

#include "Common/Decode.h"
#include "Common/Constants.h"
#include "Common/Shifts.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

void Arm::ExecuteDataProcessingInstructionShift(uint32_t instruction) {
    const auto& operands = ReadOperandsInstructionShift(instruction);
    switch (InstructionDataProcessingOpcode(instruction)) {
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

auto Arm::ReadOperandsInstructionShift(uint32_t instruction) -> DataProcessingOperands {
    const auto mode = GetMode();
    const auto m = InstructionRegisterRm(instruction);
    const auto n = InstructionRegisterRn(instruction);
    const auto s = InstructionShiftAmount(instruction);
    const auto rm = GetRegisterWithPSR(mode, m);
    const auto rn = GetRegisterWithoutPSR(mode, n);
    CycleS();
    PrefetchInstruction();
    IncrementPC();
    const auto op1 = rn;
    uint32_t op2, carry;
    ReadOperand2InstructionShift(instruction, rm, s, op2, carry);
    return {
        .op1 = op1,
        .op2 = op2,
        .carry = carry
    };
}

auto Arm::ReadOperand2InstructionShift(
    uint32_t instruction,
    uint32_t m,
    uint32_t s,
    uint32_t& op2,
    uint32_t& carry) const -> void {
    switch (InstructionShiftType(instruction)) {
        case SHIFT_TYPE_LSL: {
            if (!s) {
                op2 = m;
                carry = LogicalShiftLeft(GetC(), 31u);
            } else {
                op2 = LogicalShiftLeft(m, s);
                carry = LogicalShiftLeft(m, s - 1u);
            }
            return;
        }
        case SHIFT_TYPE_LSR: {
            if (!s) {
                op2 = 0u;
                carry = m;
            } else {
                op2 = LogicalShiftRight(m, s);
                carry = LogicalShiftLeft(m, 32u - s);
            }
            return;
        }
        case SHIFT_TYPE_ASR: {
            if (!s) {
                op2 = ArithmeticShiftRight(m, 31u);
                carry = m;
            } else {
                op2 = ArithmeticShiftRight(m, s);
                carry = LogicalShiftLeft(m, 32u - s);
            }
            return;
        }
        case SHIFT_TYPE_ROR: {
            if (!s) {
                op2 = LogicalShiftRight(m, 1u) + LogicalShiftLeft(GetC(), 31u);
                carry = LogicalShiftLeft(m, 31u);
            } else {
                op2 = RotateRight(m, s);
                carry = LogicalShiftLeft(m, 32u - s);
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