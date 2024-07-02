#include "Arm/ArmTest.h"

class AddOperand2InstructionShiftTest: public ArmTest {};

TEST_F(AddOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $00000011"
    });
    When({
        "ADD R2, R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000090",
        "R3 is $00000008",
        "R4 is $00000011"
    });
}

TEST_F(AddOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000000"
    });
    When({
        "ADD R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000008",
        "R3 is $00000008",
        "R4 is $80000000"
    });
}

TEST_F(AddOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000000"
    });
    When({
        "ADD R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000008",
        "R3 is $00000008",
        "R4 is $80000000"
    });
}

TEST_F(AddOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000001"
    });
    When({
        "ADD R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000008",
        "R3 is $00000008",
        "R4 is $80000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00001000",
        "R3 is $00001000",
        "R4 is $00000001"
    });
    When({
        "ADD R2, R3, R4, RRX"
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

TEST_F(AddOperand2InstructionShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "ADD R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000020",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "ADD R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000020",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $00000001"
    });
    When({
        "ADDS R2, R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000020",
        "R3 is $80000000",
        "R4 is $00000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "ADDS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000000"
    });
}

TEST_F(AddOperand2InstructionShiftTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $C0000000",
        "R4 is $40000001"
    });
    When({
        "ADDS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $C0000000",
        "R4 is $40000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000",
        "R4 is $40000001"
    });
    When({
        "ADDS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000000",
        "R4 is $40000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $80000001"
    });
    When({
        "ADDS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $80000000",
        "R4 is $80000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "ADD R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "ADDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCV,USR",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "ADDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "ADDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "ADDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(AddOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000"
    });
    When({
        "ADD R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R4 is $00000000"
    });
}

TEST_F(AddOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000"
    });
    When({
        "ADD R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $2C00100B",
        "R4 is $00000000"
    });
}