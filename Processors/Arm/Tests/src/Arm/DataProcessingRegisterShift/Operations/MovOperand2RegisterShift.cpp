#include "Arm/ArmTest.h"

class MovOperand2RegisterShiftTest: public ArmTest {};

TEST_F(MovOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "MOV R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000088",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(MovOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "MOV R2, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(MovOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "MOV R2, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(MovOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "MOV R2, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(MovOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
    When({
        "MOVS R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000000",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
}

TEST_F(MovOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "MOVS R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "MOV R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "MOVS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "MOVS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "MOVS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "MOVS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R5 is $00000000"
    });
    When({
        "MOV R2, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100F",
        "R5 is $00000000"
    });
}

TEST_F(MovOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R5 is $00000001"
    });
    When({
        "MOV R2, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00000100",
        "R5 is $00000001"
    });
}