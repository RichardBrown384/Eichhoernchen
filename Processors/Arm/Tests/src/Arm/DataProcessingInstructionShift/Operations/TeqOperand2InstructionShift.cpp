#include "Arm/ArmTest.h"

class TeqOperand2InstructionShiftTest: public ArmTest {};

TEST_F(TeqOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00001080",
        "R4 is $00000210"
    });
    When({
        "TEQ R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00001080",
        "R4 is $00000210"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $20000001",
        "R4 is $80000004"
    });
    When({
        "TEQ R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $20000001",
        "R4 is $80000004"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $20000001",
        "R4 is $80000004"
    });
    When({
        "TEQ R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $20000001",
        "R4 is $80000004"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000001",
        "R4 is $80000005"
    });
    When({
        "TEQ R3, R4, ROR #1"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $00000001",
        "R4 is $80000005"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000001",
        "R4 is $80000005"
    });
    When({
        "TEQ R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000001",
        "R4 is $80000005"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000001",
        "R4 is $00000003"
    });
    When({
        "TEQ R3, R4, LSL #30"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $40000001",
        "R4 is $00000003"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00FF5500",
        "R4 is $00FF5500"
    });
    When({
        "TEQ R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00FF5500",
        "R4 is $00FF5500"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $AC002001"
    });
    When({
        "TEQP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,USR",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $AC002001"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
    When({
        "TEQP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NZCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
    When({
        "TEQP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
}

TEST_F(TeqOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
    When({
        "TEQP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
}

TEST_F(TeqOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $00001008"
    });
    When({
        "TEQ R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $00001008"
    });
}

TEST_F(TeqOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $1C00100B"
    });
    When({
        "TEQ R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $1C00100B"
    });
}