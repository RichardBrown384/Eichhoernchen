#include "Arm/ArmTest.h"

class RscOperand2RegisterShiftTest: public ArmTest {};

TEST_F(RscOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000200",
        "R4 is $00000011",
        "R5 is $00000007"
    });
    When({
        "RSC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000680",
        "R3 is $00000200",
        "R4 is $00000011",
        "R5 is $00000007"
    });
}

TEST_F(RscOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000002",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "RSC R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $1FFFFFFE",
        "R3 is $00000002",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(RscOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000002",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "RSC R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $DFFFFFFE",
        "R3 is $00000002",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(RscOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $10000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "RSC R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $4FFFFFF8",
        "R3 is $10000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(RscOperand2RegisterShiftTest, CarryFlagSetInfluencesResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500",
        "R4 is $00000001",
        "R5 is $0000000B"
    });
    When({
        "RSC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000300",
        "R3 is $00000500",
        "R4 is $00000001",
        "R5 is $0000000B"
    });
}

TEST_F(RscOperand2RegisterShiftTest, CarryFlagClearInfluencesResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000500",
        "R4 is $00000001",
        "R5 is $0000000B"
    });
    When({
        "RSC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $000002FF",
        "R3 is $00000500",
        "R4 is $00000001",
        "R5 is $0000000B"
    });
}

TEST_F(RscOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFF020",
        "R3 is $00001000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(RscOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000004",
        "R4 is $00000005",
        "R5 is $00000000"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000004",
        "R4 is $00000005",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000005",
        "R4 is $00000004",
        "R5 is $00000000"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000005",
        "R4 is $00000004",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000001",
        "R4 is $40000000",
        "R5 is $00000000"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $BFFFFFFF",
        "R3 is $80000001",
        "R4 is $40000000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000000"
    });
    When({
        "RSCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $7FFFFFFF",
        "R3 is $00000001",
        "R4 is $80000000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
    When({
        "RSC R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is 0,USR",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
    When({
        "RSCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NV,USR",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
    When({
        "RSCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
    When({
        "RSCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
    When({
        "RSCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NVIF,SVC",
        "PC is $00002004",
        "R3 is $10000001",
        "R4 is $AC002000",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
    When({
        "RSC R2, R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R4 is $2C00100F",
        "R5 is $00000000"
    });
    When({
        "RSC R2, R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $2C00100F",
        "R5 is $00000000"
    });
}

TEST_F(RscOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000100",
        "R5 is $00000001"
    });
    When({
        "RSC R2, R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is CIF,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $00000100",
        "R5 is $00000001"
    });
}