#include "Arm/ArmTest.h"

class EorOperand2RegisterShiftTest: public ArmTest {};

TEST_F(EorOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001080",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "EOR R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R3 is $00001080",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(EorOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
    When({
        "EOR R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000000",
        "R3 is $00000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
}

TEST_F(EorOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
    When({
        "EOR R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000000",
        "R3 is $00000001",
        "R4 is $80000004",
        "R5 is $00000002"
    });
}

TEST_F(EorOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000005",
        "R5 is $00000002"
    });
    When({
        "EOR R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R3 is $00000001",
        "R4 is $80000005",
        "R5 is $00000002"
    });
}

TEST_F(EorOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000001",
        "R4 is $00000003",
        "R5 is $0000001E"
    });
    When({
        "EORS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000001",
        "R4 is $00000003",
        "R5 is $0000001E"
    });
}

TEST_F(EorOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00FF5500",
        "R4 is $00FF5500",
        "R5 is $00000000"
    });
    When({
        "EORS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00FF5500",
        "R4 is $00FF5500",
        "R5 is $00000000"
    });
}

TEST_F(EorOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
    When({
        "EOR R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is N,USR",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
}

TEST_F(EorOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "EORS R15, R3, R4, LSL R5"
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

TEST_F(EorOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
    When({
        "EORS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NZCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $FC002001",
        "R5 is $00000000"
    });
}

TEST_F(EorOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "EORS R15, R3, R4, LSL R5"
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

TEST_F(EorOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "EORS R15, R3, R4, LSL R5"
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

TEST_F(EorOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "EOR R2, R15, R4, LSL R5"
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

TEST_F(EorOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "EOR R2, R4, R15, LSL R5"
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

TEST_F(EorOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000001"
    });
    When({
        "EOR R2, R4, R5, LSL R15"
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