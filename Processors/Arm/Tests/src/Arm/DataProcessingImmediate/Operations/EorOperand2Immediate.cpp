#include "Arm/ArmTest.h"

class EorOperand2ImmediateTest: public ArmTest {};

TEST_F(EorOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001080"
    });
    When({
        "EOR R2, R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R3 is $00001080"
    });
}

TEST_F(EorOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000001"
    });
    When({
        "EORS R2, R3, #0xC0000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000001"
    });
}

TEST_F(EorOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00FF0000"
    });
    When({
        "EORS R2, R3, #0x00FF0000"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00FF0000"
    });
}

TEST_F(EorOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $FC002001"
    });
    When({
        "EOR R15, R3, #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is N,USR",
        "PC is $00002008",
        "R3 is $FC002001"
    });
}

TEST_F(EorOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $AC002001"
    });
    When({
        "EORS R15, R3, #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R3 is $AC002001"
    });
}

TEST_F(EorOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $FC002001"
    });
    When({
        "EORS R15, R3, #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCVIF,FIQ",
        "PC is $00002008",
        "R3 is $FC002001"
    });
}

TEST_F(EorOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $BC002001"
    });
    When({
        "EORS R15, R3, #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $BC002001"
    });
}

TEST_F(EorOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $BC002001"
    });
    When({
        "EORS R15, R3, #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $BC002001"
    });
}

TEST_F(EorOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "EOR R2, R15, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008"
    });
}