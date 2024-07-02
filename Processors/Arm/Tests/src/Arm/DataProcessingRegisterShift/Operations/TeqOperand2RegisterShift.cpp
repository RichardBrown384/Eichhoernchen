#include "Arm/ArmTest.h"

class TeqOperand2RegisterShiftTest: public ArmTest {};

TEST_F(TeqOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00001080",
        "R4 is $00000210",
        "R5 is $00000003"
    });
    When({
        "TEQ R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00001080",
        "R4 is $00000210",
        "R5 is $00000003"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $20000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
    When({
        "TEQ R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $20000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $20000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
    When({
        "TEQ R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $20000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000001",
        "R4 is $80000005",
        "R5 is $00000001"
    });
    When({
        "TEQ R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $00000001",
        "R4 is $80000005",
        "R5 is $00000001"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000001",
        "R4 is $00000003",
        "R5 is $0000001E"
    });
    When({
        "TEQ R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $40000001",
        "R4 is $00000003",
        "R5 is $0000001E"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00FF5500",
        "R4 is $00FF5500",
        "R5 is $00000000"
    });
    When({
        "TEQ R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00FF5500",
        "R4 is $00FF5500",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "TEQP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,USR",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
    When({
        "TEQP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NZCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "TEQP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "TEQP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
    When({
        "TEQ R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $1C00100F",
        "R5 is $00000000"
    });
    When({
        "TEQ R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $1C00100F",
        "R5 is $00000000"
    });
}

TEST_F(TeqOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $00000100",
        "R5 is $00000001"
    });
    When({
        "TEQ R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $00000100",
        "R5 is $00000001"
    });
}