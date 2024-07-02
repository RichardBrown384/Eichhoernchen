#include "Arm/ArmTest.h"

class RscOperand2ImmediateTest: public ArmTest {};

TEST_F(RscOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200"
    });
    When({
        "RSC R2, R3, #0x880"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000680",
        "R3 is $00000200"
    });
}

TEST_F(RscOperand2ImmediateTest, CarryFlagSetInfluencesResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500"
    });
    When({
        "RSC R2, R3, #0x800"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000300",
        "R3 is $00000500"
    });
}

TEST_F(RscOperand2ImmediateTest, CarryFlagClearInfluencesResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500"
    });
    When({
        "RSC R2, R3, #0x800"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $000002FF",
        "R3 is $00000500"
    });
}

TEST_F(RscOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000"
    });
    When({
        "RSCS R2, R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFF020",
        "R3 is $00001000"
    });
}

TEST_F(RscOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000005"
    });
    When({
        "RSCS R2, R3, #5"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000005"
    });
}

TEST_F(RscOperand2ImmediateTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004"
    });
    When({
        "RSCS R2, R3, #5"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000004"
    });
}

TEST_F(RscOperand2ImmediateTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005"
    });
    When({
        "RSCS R2, R3, #4"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000005"
    });
}

TEST_F(RscOperand2ImmediateTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000001"
    });
    When({
        "RSCS R2, R3, #0x40000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $BFFFFFFF",
        "R3 is $80000001"
    });
}

TEST_F(RscOperand2ImmediateTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001"
    });
    When({
        "RSCS R2, R3, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $7FFFFFFF",
        "R3 is $00000001"
    });
}

TEST_F(RscOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001"
    });
    When({
        "RSC R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00000004",
        "R3 is $10000001"
    });
}

TEST_F(RscOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001"
    });
    When({
        "RSCS R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NV,USR",
        "PC is $00000004",
        "R3 is $10000001"
    });
}

TEST_F(RscOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R3 is $10000001"
    });
    When({
        "RSCS R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,USR",
        "PC is $00000008",
        "R3 is $10000001"
    });
}

TEST_F(RscOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R3 is $10000001"
    });
    When({
        "RSCS R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,USR",
        "PC is $00000008",
        "R3 is $10000001"
    });
}

TEST_F(RscOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $10000001"
    });
    When({
        "RSCS R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NVIF,USR",
        "PC is $00000008",
        "R3 is $10000001"
    });
}

TEST_F(RscOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001"
    });
    When({
        "RSC R2, R15, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $FFFFEFF8"
    });
}