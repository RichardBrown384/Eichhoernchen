#include "Arm/ArmTest.h"

class AddOperand2ImmediateTest: public ArmTest {};

TEST_F(AddOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008"
    });
    When({
        "ADD R2, R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000090",
        "R3 is $00000008"
    });
}

TEST_F(AddOperand2ImmediateTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "ADD R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000020",
        "R3 is $00000000"
    });
}

TEST_F(AddOperand2ImmediateTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "ADD R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000020",
        "R3 is $00000000"
    });
}

TEST_F(AddOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000"
    });
    When({
        "ADDS R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000020",
        "R3 is $80000000"
    });
}

TEST_F(AddOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000"
    });
    When({
        "ADDS R2, R3, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000"
    });
}

TEST_F(AddOperand2ImmediateTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $C0000000"
    });
    When({
        "ADDS R2, R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $C0000000"
    });
}

TEST_F(AddOperand2ImmediateTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000"
    });
    When({
        "ADDS R2, R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000000"
    });
}

TEST_F(AddOperand2ImmediateTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000"
    });
    When({
        "ADDS R2, R3, #0x80000001"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $80000000"
    });
}

TEST_F(AddOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ADD R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(AddOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ADDS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCV,USR",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(AddOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ADDS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(AddOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ADDS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(AddOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "ADDS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000"
    });
}

TEST_F(AddOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "ADD R2, R15, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008"
    });
}