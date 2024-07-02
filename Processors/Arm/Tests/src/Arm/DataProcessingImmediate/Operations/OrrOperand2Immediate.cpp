#include "Arm/ArmTest.h"

class OrrOperand2ImmediateTest: public ArmTest {};

TEST_F(OrrOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000"
    });
    When({
        "ORR R2, R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001088",
        "R3 is $00001000"
    });
}

TEST_F(OrrOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001"
    });
    When({
        "ORRS R2, R3, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $00000001"
    });
}

TEST_F(OrrOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000"
    });
    When({
        "ORRS R2, R3, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000"
    });
}

TEST_F(OrrOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FC002000"
    });
    When({
        "ORR R15, R3, #1"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $FC002000"
    });
}

TEST_F(OrrOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ORRS R15, R3, #1"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(OrrOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is V,IRQ",
        "PC is $00001008",
        "R3 is $BC002000"
    });
    When({
        "ORRS R15, R3, #1"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $BC002000"
    });
}

TEST_F(OrrOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $BC002000"
    });
    When({
        "ORRS R15, R3, #1"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $BC002000"
    });
}

TEST_F(OrrOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $BC002000"
    });
    When({
        "ORRS R15, R3, #1"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $BC002000"
    });
}

TEST_F(OrrOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "ORR R2, R15, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008"
    });
}