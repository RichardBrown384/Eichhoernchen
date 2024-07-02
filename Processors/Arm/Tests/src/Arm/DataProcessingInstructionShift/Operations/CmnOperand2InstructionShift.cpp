#include "Arm/ArmTest.h"

class CmnOperand2InstructionShiftTest: public ArmTest {};

TEST_F(CmnOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000008",
        "R4 is $08000000"
    });
    When({
        "CMN R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000008",
        "R4 is $08000000"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000000"
    });
    When({
        "CMN R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000000"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000008",
        "R4 is $80000000"
    });
    When({
        "CMN R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000008",
        "R4 is $80000000"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R3 is $00000008",
        "R4 is $80000001"
    });
    When({
        "CMN R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000008",
        "R4 is $80000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R3 is $00000008",
        "R4 is $80000001"
    });
    When({
        "CMN R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000008",
        "R4 is $80000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $3FFFFFFF"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $3FFFFFFF"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $3FFFFFFF"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $3FFFFFFF"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $00000001"
    });
    When({
        "CMN R3, R4, LSL #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $00000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $00000000"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $C0000000",
        "R4 is $40000001"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $C0000000",
        "R4 is $40000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $40000001"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $40000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000001"
    });
    When({
        "CMN R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMNP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCV,USR",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMNP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMNP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
    When({
        "CMNP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001"
    });
}

TEST_F(CmnOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R4 is $FFFFEFF8"
    });
    When({
        "CMN R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $FFFFEFF8"
    });
}

TEST_F(CmnOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R4 is $73FFEFF5"
    });
    When({
        "CMN R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $73FFEFF5"
    });
}