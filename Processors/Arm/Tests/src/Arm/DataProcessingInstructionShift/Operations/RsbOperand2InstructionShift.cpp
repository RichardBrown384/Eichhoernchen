#include "Arm/ArmTest.h"

class RsbOperand2InstructionShiftTest: public ArmTest {};

TEST_F(RsbOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200",
        "R4 is $00000011"
    });
    When({
        "RSB R2, R3, R4, LSL #7"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000680",
        "R3 is $00000200",
        "R4 is $00000011"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000002",
        "R4 is $80000000"
    });
    When({
        "RSB R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $1FFFFFFE",
        "R3 is $00000002",
        "R4 is $80000000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000002",
        "R4 is $80000000"
    });
    When({
        "RSB R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $DFFFFFFE",
        "R3 is $00000002",
        "R4 is $80000000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $10000008",
        "R4 is $80000001"
    });
    When({
        "RSB R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $4FFFFFF8",
        "R3 is $10000008",
        "R4 is $80000001"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
    When({
        "RSB R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $7FFFF000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500",
        "R4 is $00000001"
    });
    When({
        "RSB R2, R3, R4, LSL #11"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000300",
        "R3 is $00000500",
        "R4 is $00000001"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500",
        "R4 is $00000001"
    });
    When({
        "RSB R2, R3, R4, LSL #11"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000300",
        "R3 is $00000500",
        "R4 is $00000001"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
    When({
        "RSBS R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFF020",
        "R3 is $00001000",
        "R4 is $00000001"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "RSBS R2, R3, R4, LSL #0"
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

TEST_F(RsbOperand2InstructionShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004",
        "R4 is $00000005"
    });
    When({
        "RSBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000004",
        "R4 is $00000005"
    });
}

TEST_F(RsbOperand2InstructionShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005",
        "R4 is $00000004"
    });
    When({
        "RSBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000005",
        "R4 is $00000004"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000001",
        "R4 is $40000000"
    });
    When({
        "RSBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $BFFFFFFF",
        "R3 is $80000001",
        "R4 is $40000000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000000"
    });
    When({
        "RSBS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $7FFFFFFF",
        "R3 is $00000001",
        "R4 is $80000000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
    When({
        "RSB R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
    When({
        "RSBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NV,USR",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
    When({
        "RSBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
    When({
        "RSBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
    When({
        "RSBS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000"
    });
}

TEST_F(RsbOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $00001008"
    });
    When({
        "RSB R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $00001008"
    });
}

TEST_F(RsbOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $2C00100B"
    });
    When({
        "RSB R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $2C00100B"
    });
}