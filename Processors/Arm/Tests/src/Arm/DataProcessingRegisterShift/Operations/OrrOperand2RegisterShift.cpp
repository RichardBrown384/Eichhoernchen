#include "Arm/ArmTest.h"

class OrrOperand2RegisterShiftTest: public ArmTest {};

TEST_F(OrrOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "ORR R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001088",
        "R3 is $00001000",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "ORR R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000001",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "ORR R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000001",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "ORR R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000001",
        "R3 is $00000001",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
    When({
        "ORRS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ORRS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
    When({
        "ORR R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "ORRS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is V,IRQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "ORRS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "ORRS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "ORRS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ORR R2, R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $0000100C",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ORR R2, R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100F",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(OrrOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000001"
    });
    When({
        "ORR R2, R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00000100",
        "R4 is $00000000",
        "R5 is $00000001"
    });
}