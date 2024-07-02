#include <cstdint>

#include "Common/Decode.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

void Arm::ApplyArithmeticFlags(uint32_t op1, uint32_t op2, uint32_t result) {
    const auto eor = op1 ^ op2;
    const auto overflow = ~eor & (op1 ^ result);
    const auto carry = eor ^ result ^ overflow;
    SetN(BitSet(result, 31u));
    SetZ(!result);
    SetC(BitSet(carry, 31u));
    SetV(BitSet(overflow, 31u));
}

void Arm::ApplyArithmeticOperation(uint32_t instruction, uint32_t op1, uint32_t op2, uint32_t result) {
    const auto psrBit = InstructionDataProcessingPsrBit(instruction);
    const auto d = InstructionRegisterRd(instruction);
    if (IsPC(d)) {
        if (psrBit) {
            DeconstructPSR(result);
        }
        Branch(result);
        return;
    }
    if (psrBit) {
        ApplyArithmeticFlags(op1, op2, result);
    }
    SetRegister(GetMode(), d, result);
}

void Arm::ApplyArithmeticCompareOperation(uint32_t instruction, uint32_t op1, uint32_t op2, uint32_t result) {
    const auto psrBit = InstructionDataProcessingPsrBit(instruction);
    const auto d = InstructionRegisterRd(instruction);
    if (psrBit) {
        if (IsPC(d)) {
            DeconstructPSR(result);
            return;
        }
        ApplyArithmeticFlags(op1, op2, result);
    }
}

void Arm::ApplyLogicalFlags(uint32_t result, uint32_t carry) {
    SetN(BitSet(result, 31u));
    SetZ(!result);
    SetC(BitSet(carry, 31u));
}

void Arm::ApplyLogicalOperation(uint32_t instruction, uint32_t result, uint32_t carry) {
    const auto psrBit = InstructionDataProcessingPsrBit(instruction);
    const auto d = InstructionRegisterRd(instruction);
    if (IsPC(d)) {
        if (psrBit) {
            DeconstructPSR(result);
        }
        Branch(result);
        return;
    }
    if (psrBit) {
        ApplyLogicalFlags(result, carry);
    }
    SetRegister(GetMode(), d, result);
}

void Arm::ApplyLogicalCompareOperation(uint32_t instruction, uint32_t result, uint32_t carry) {
    const auto psrBit = InstructionDataProcessingPsrBit(instruction);
    const auto d = InstructionRegisterRd(instruction);
    if (psrBit) {
        if (IsPC(d)) {
            DeconstructPSR(result);
            return;
        }
        ApplyLogicalFlags(result, carry);
    }
}

void Arm::ApplyMultiplyAccumulateFlags(uint32_t result) {
    SetN(BitSet(result, 31u));
    SetZ(!result);
}

void Arm::ApplyMultiplyAccumulateOperation(uint32_t instruction, uint32_t result) {
    const auto psrBit = InstructionDataProcessingPsrBit(instruction);
    const auto d = InstructionRegisterRn(instruction);
    if (IsPC(d)) {}
    else {
        if (psrBit) {
            ApplyMultiplyAccumulateFlags(result);
        }
        SetRegister(GetMode(), d, result);
    }
}

void Arm::And(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, op1 & op2, carry);
}

void Arm::Eor(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, op1 ^ op2, carry);
}

void Arm::Sub(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op1, ~op2, op1 + (~op2) + 1u);
}

void Arm::Rsb(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op2, ~op1, op2 + (~op1) + 1u);
}

void Arm::Add(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op1, op2, op1 + op2);
}

void Arm::Adc(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op1, op2, op1 + op2 + GetC());
}

void Arm::Sbc(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op1, ~op2, op1 + (~op2) + GetC());
}

void Arm::Rsc(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticOperation(instruction, op2, ~op1, op2 + (~op1) + GetC());
}

void Arm::Tst(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalCompareOperation(instruction, op1 & op2, carry);
}

void Arm::Teq(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalCompareOperation(instruction, op1 ^ op2, carry);
}

void Arm::Cmp(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticCompareOperation(instruction, op1, ~op2, op1 + (~op2) + 1u);
}

void Arm::Cmn(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyArithmeticCompareOperation(instruction, op1, op2, op1 + op2);
}

void Arm::Orr(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, op1 | op2, carry);
}

void Arm::Mov(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, op2, carry);
}

void Arm::Bic(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, op1 & (~op2), carry);
}

void Arm::Mvn(uint32_t instruction, const DataProcessingOperands& operands) {
    const auto& [op1, op2, carry] = operands;
    ApplyLogicalOperation(instruction, ~op2, carry);
}

void Arm::Mul(uint32_t instruction, const MultiplyAccumulateOperands& operands) {
    const auto& [op1, op2, accumulator] = operands;
    ApplyMultiplyAccumulateOperation(instruction, op1 * op2);
}

void Arm::Mla(uint32_t instruction, const MultiplyAccumulateOperands& operands) {
    const auto& [op1, op2, accumulator] = operands;
    ApplyMultiplyAccumulateOperation(instruction, op1 * op2 + accumulator);
}