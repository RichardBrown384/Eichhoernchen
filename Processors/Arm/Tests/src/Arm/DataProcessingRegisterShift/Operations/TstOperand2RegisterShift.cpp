#include "Arm/ArmTest.h"

class TstOperand2RegisterShiftTest: public ArmTest {};

TEST_F(TstOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $10000011",
        "R5 is $00000003"
    });
    When({
        "TST R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $10000011",
        "R5 is $00000003"
    });
}

TEST_F(TstOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is NZ,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "TST R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(TstOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "TST R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(TstOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000007",
        "R5 is $00000002"
    });
    When({
        "TST R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000007",
        "R5 is $00000002"
    });
}

TEST_F(TstOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
    When({
        "TST R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $00000001",
        "R5 is $0000001F"
    });
}

TEST_F(TstOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A",
        "R5 is $00000000"
    });
    When({
        "TST R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
    When({
        "TSTP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,USR",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "TSTP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "TSTP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
    When({
        "TSTP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $FFFFEFF3",
        "R5 is $00000000"
    });
    When({
        "TST R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $FFFFEFF3",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $E3FFEFF0",
        "R5 is $00000000"
    });
    When({
        "TST R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $E3FFEFF0",
        "R5 is $00000000"
    });
}

TEST_F(TstOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $FFFFFFFF",
        "R5 is $00800000"
    });
    When({
        "TST R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R4 is $FFFFFFFF",
        "R5 is $00800000"
    });
}