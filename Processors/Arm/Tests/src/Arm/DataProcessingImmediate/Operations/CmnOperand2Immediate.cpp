#include "Arm/ArmTest.h"

class CmnOperand2ImmediateTest: public ArmTest {};

TEST_F(CmnOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000008"
    });
    When({
        "CMN R3, #0x40000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000008"
    });
}

TEST_F(CmnOperand2ImmediateTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $3FFFFFFF"
    });
    When({
        "CMN R3, #0x40000000"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $3FFFFFFF"
    });
}

TEST_F(CmnOperand2ImmediateTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $3FFFFFFF"
    });
    When({
        "CMN R3, #0x40000000"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $3FFFFFFF"
    });
}

TEST_F(CmnOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000"
    });
    When({
        "CMN R3, #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $80000000"
    });
}

TEST_F(CmnOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000"
    });
    When({
        "CMN R3, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00000000"
    });
}

TEST_F(CmnOperand2ImmediateTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $C0000000"
    });
    When({
        "CMN R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $C0000000"
    });
}

TEST_F(CmnOperand2ImmediateTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000"
    });
    When({
        "CMN R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000"
    });
}

TEST_F(CmnOperand2ImmediateTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000"
    });
    When({
        "CMN R3, #0x80000001"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000"
    });
}

TEST_F(CmnOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMNP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCV,USR",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmnOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMNP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmnOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMNP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmnOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMNP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmnOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008"
    });
    When({
        "CMN R15, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NIF,SVC",
        "PC is $0000100C"
    });
}