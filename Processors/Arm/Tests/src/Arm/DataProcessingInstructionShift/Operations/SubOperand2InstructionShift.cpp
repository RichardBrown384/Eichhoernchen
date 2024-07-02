#include "Arm/ArmTest.h"

class SubOperand2InstructionShiftTest: public ArmTest {};

TEST_F(SubOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200",
        "R4 is $00000011"
    });
    When({
        "SUB R2, R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000178",
        "R3 is $00000200",
        "R4 is $00000011"
    });
}

TEST_F(SubOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $80000000"
    });
    When({
        "SUB R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R3 is $80000000",
        "R4 is $80000000"
    });
}

TEST_F(SubOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
    When({
        "SUB R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $10000000",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
}

TEST_F(SubOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
    When({
        "SUB R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $90000008",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
    When({
        "SUB R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $80001000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "SUB R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $000000E0",
        "R3 is $00000100",
        "R4 is $00000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "SUB R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $000000E0",
        "R3 is $00000100",
        "R4 is $00000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "SUBS R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFE0",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "SUBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000000"
    });
}

TEST_F(SubOperand2InstructionShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005",
        "R4 is $00000004"
    });
    When({
        "SUBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000005",
        "R4 is $00000004"
    });
}

TEST_F(SubOperand2InstructionShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004",
        "R4 is $00000005"
    });
    When({
        "SUBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000004",
        "R4 is $00000005"
    });
}

TEST_F(SubOperand2InstructionShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000",
        "R4 is $80000001"
    });
    When({
        "SUBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $BFFFFFFF",
        "R3 is $40000000",
        "R4 is $80000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $00000001"
    });
    When({
        "SUBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $7FFFFFFF",
        "R3 is $80000000",
        "R4 is $00000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SUB R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SUBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NV,USR",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SUBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SUBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SUBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SubOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000"
    });
    When({
        "SUB R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R4 is $00000000"
    });
}

TEST_F(SubOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $2C00100B"
    });
    When({
        "SUB R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $2C00100B"
    });
}