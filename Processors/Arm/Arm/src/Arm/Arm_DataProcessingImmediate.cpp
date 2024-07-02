#include <cstdint>

#include "Common/Decode.h"
#include "Common/Constants.h"
#include "Common/Shifts.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

void Arm::ExecuteDataProcessingImmediate(uint32_t instruction) {
    const auto& operands = ReadOperandsImmediate(instruction);
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

auto Arm::ReadOperandsImmediate(uint32_t instruction) -> DataProcessingOperands {
    const auto mode = GetMode();
    const auto n = InstructionRegisterRn(instruction);
    const auto s = InstructionRotateAmount(instruction);
    const auto imm = InstructionImmediate(instruction);
    const auto rn = GetRegisterWithoutPSR(mode, n);
    CycleS();
    PrefetchInstruction();
    IncrementPC();
    const auto op1 = rn;
    uint32_t op2, carry;
    ReadOperand2Immediate(imm, 2u * s, op2, carry);
    return {
        .op1 = op1,
        .op2 = op2,
        .carry = carry
    };
}

auto Arm::ReadOperand2Immediate(
    uint32_t imm,
    uint32_t s,
    uint32_t& op2,
    uint32_t& carry) const -> void {
    if (!s) {
        op2 = imm;
        carry = LogicalShiftLeft(GetC(), 31u);
    } else {
        op2 = RotateRight(imm, s);
        carry = LogicalShiftLeft(imm, 32u - s);
    }
}