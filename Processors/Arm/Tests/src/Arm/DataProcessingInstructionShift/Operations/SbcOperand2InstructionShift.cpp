#include "Arm/ArmTest.h"

class SbcOperand2InstructionShiftTest: public ArmTest {};

TEST_F(SbcOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200",
        "R4 is $00000011"
    });
    When({
        "SBC R2, R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000178",
        "R3 is $00000200",
        "R4 is $00000011"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $80000000"
    });
    When({
        "SBC R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R3 is $80000000",
        "R4 is $80000000"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
    When({
        "SBC R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $10000000",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
    When({
        "SBC R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $90000008",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
    When({
        "SBC R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000FFF",
        "R3 is $00001000",
        "R4 is $00000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, CarryFlagSetInfluencesResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "SBC R2, R3, R4, LSL #5"
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

TEST_F(SbcOperand2InstructionShiftTest, CarryFlagClearInfluencesResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "SBC R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $000000DF",
        "R3 is $00000100",
        "R4 is $00000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "SBCS R2, R3, R4, LSL #5"
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

TEST_F(SbcOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "SBCS R2, R3, R4, LSL #0"
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

TEST_F(SbcOperand2InstructionShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005",
        "R4 is $00000004"
    });
    When({
        "SBCS R2, R3, R4, LSL #0"
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

TEST_F(SbcOperand2InstructionShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004",
        "R4 is $00000005"
    });
    When({
        "SBCS R2, R3, R4, LSL #0"
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

TEST_F(SbcOperand2InstructionShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000",
        "R4 is $80000001"
    });
    When({
        "SBCS R2, R3, R4, LSL #0"
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

TEST_F(SbcOperand2InstructionShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $00000001"
    });
    When({
        "SBCS R2, R3, R4, LSL #0"
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

TEST_F(SbcOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SBC R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SBCS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NV,USR",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SBCS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SBCS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "SBCS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(SbcOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $00001008"
    });
    When({
        "SBC R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $00001008"
    });
}

TEST_F(SbcOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $2C00100B"
    });
    When({
        "SBC R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $2C00100B"
    });
}