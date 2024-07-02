#include "Arm/ArmTest.h"

class CmnOperand2RegisterShiftTest: public ArmTest {};

TEST_F(CmnOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000008",
        "R4 is $08000000",
        "R5 is $00000003"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000008",
        "R4 is $08000000",
        "R5 is $00000003"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "CMN R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "CMN R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R3 is $00000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "CMN R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $3FFFFFFF",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $3FFFFFFF",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $3FFFFFFF",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $3FFFFFFF",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $C0000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $C0000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
    When({
        "CMN R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMNP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCV,USR",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMNP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMNP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMNP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R4 is $FFFFEFF4",
        "R5 is $00000000"
    });
    When({
        "CMN R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $FFFFEFF4",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R4 is $73FFEFF1",
        "R5 is $00000000"
    });
    When({
        "CMN R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $73FFEFF1",
        "R5 is $00000000"
    });
}

TEST_F(CmnOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $80000000",
        "R5 is $00800000"
    });
    When({
        "CMN R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCVIF,SVC",
        "PC is $0000100C",
        "R4 is $80000000",
        "R5 is $00800000"
    });
}