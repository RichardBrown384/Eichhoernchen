#include "Arm/ArmTest.h"

class CmpOperand2ImmediateTest: public ArmTest {};

TEST_F(CmpOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000200"
    });
    When({
        "CMP R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000200"
    });
}

TEST_F(CmpOperand2ImmediateTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000100"
    });
    When({
        "CMP R3, #0x100"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100"
    });
}

TEST_F(CmpOperand2ImmediateTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000100"
    });
    When({
        "CMP R3, #0x100"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100"
    });
}

TEST_F(CmpOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000"
    });
    When({
        "CMP R3, #0x20"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000000"
    });
}

TEST_F(CmpOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000002"
    });
    When({
        "CMP R3, #0x2"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000002"
    });
}

TEST_F(CmpOperand2ImmediateTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000005"
    });
    When({
        "CMP R3, #4"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000005"
    });
}

TEST_F(CmpOperand2ImmediateTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000004"
    });
    When({
        "CMP R3, #5"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000004"
    });
}

TEST_F(CmpOperand2ImmediateTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000"
    });
    When({
        "CMP R3, #0x80000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000"
    });
}

TEST_F(CmpOperand2ImmediateTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000"
    });
    When({
        "CMP R3, #1"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000"
    });
}

TEST_F(CmpOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMPP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,USR",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmpOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMPP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmpOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMPP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmpOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000"
    });
    When({
        "CMPP R3, #0x10000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000"
    });
}

TEST_F(CmpOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00000108"
    });
    When({
        "CMP R15, #0x108"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCIF,SVC",
        "PC is $0000010C"
    });
}