#include "Arm/ArmTest.h"

class SubOperand2ImmediateTest: public ArmTest {};

TEST_F(SubOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200"
    });
    When({
        "SUB R2, R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000178",
        "R3 is $00000200"
    });
}

TEST_F(SubOperand2ImmediateTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100"
    });
    When({
        "SUB R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $000000E0",
        "R3 is $00000100"
    });
}

TEST_F(SubOperand2ImmediateTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000100"
    });
    When({
        "SUB R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $000000E0",
        "R3 is $00000100"
    });
}

TEST_F(SubOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "SUBS R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFE0",
        "R3 is $00000000"
    });
}

TEST_F(SubOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000001"
    });
    When({
        "SUBS R2, R3, #1"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000001"
    });
}

TEST_F(SubOperand2ImmediateTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005"
    });
    When({
        "SUBS R2, R3, #4"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000005"
    });
}

TEST_F(SubOperand2ImmediateTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004"
    });
    When({
        "SUBS R2, R3, #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000004"
    });
}

TEST_F(SubOperand2ImmediateTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000"
    });
    When({
        "SUBS R2, R3, #0x80000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $BFFFFFFF",
        "R3 is $40000000"
    });
}

TEST_F(SubOperand2ImmediateTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000"
    });
    When({
        "SUBS R2, R3, #1"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $7FFFFFFF",
        "R3 is $80000000"
    });
}

TEST_F(SubOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "SUB R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002004",
        "R3 is $AC002000"
    });
}

TEST_F(SubOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "SUBS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NV,USR",
        "PC is $00002004",
        "R3 is $AC002000"
    });
}

TEST_F(SubOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "SUBS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000"
    });
}

TEST_F(SubOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "SUBS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000"
    });
}

TEST_F(SubOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "SUBS R15, R3, #0x10000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $AC002000"
    });
}

TEST_F(SubOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "SUB R2, R15, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008"
    });
}