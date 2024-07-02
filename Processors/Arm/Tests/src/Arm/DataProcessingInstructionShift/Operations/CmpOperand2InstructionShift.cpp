#include "Arm/ArmTest.h"

class CmpOperand2InstructionShiftTest: public ArmTest {};

TEST_F(CmpOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000200",
        "R4 is $00000011"
    });
    When({
        "CMP R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000200",
        "R4 is $00000011"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000000"
    });
    When({
        "CMP R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000000"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
    When({
        "CMP R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $F0000000",
        "R4 is $80000000"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
    When({
        "CMP R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
    When({
        "CMP R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $F0000008",
        "R4 is $80000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "CMP R3, R4, LSL #8"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100",
        "R4 is $00000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000100",
        "R4 is $00000001"
    });
    When({
        "CMP R3, R4, LSL #8"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100",
        "R4 is $00000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "CMP R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000002",
        "R4 is $00000002"
    });
    When({
        "CMP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000002",
        "R4 is $00000002"
    });
}

TEST_F(CmpOperand2InstructionShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000005",
        "R4 is $00000004"
    });
    When({
        "CMP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000005",
        "R4 is $00000004"
    });
}

TEST_F(CmpOperand2InstructionShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R4 is $00000005"
    });
    When({
        "CMP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000004",
        "R4 is $00000005"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $80000001"
    });
    When({
        "CMP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $80000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $00000001"
    });
    When({
        "CMP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $00000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMPP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,USR",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMPP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMPP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMPP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmpOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $00001008"
    });
    When({
        "CMP R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $00001008"
    });
}

TEST_F(CmpOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $2C00100B"
    });
    When({
        "CMP R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $2C00100B"
    });
}