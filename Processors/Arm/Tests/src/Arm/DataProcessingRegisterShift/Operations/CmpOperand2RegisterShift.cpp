#include "Arm/ArmTest.h"

class CmpOperand2RegisterShiftTest: public ArmTest {};

TEST_F(CmpOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000200",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000200",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "CMP R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $F0000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "CMP R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $F0000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $F0000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "CMP R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $F0000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, CarryFlagSetDoesNotInfluenceResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000100",
        "R4 is $00000001",
        "R5 is $00000008"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100",
        "R4 is $00000001",
        "R5 is $00000008"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, CarryFlagClearDoesNotInfluenceResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000100",
        "R4 is $00000001",
        "R5 is $00000008"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000100",
        "R4 is $00000001",
        "R5 is $00000008"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000002",
        "R4 is $00000002",
        "R5 is $00000000"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R3 is $00000002",
        "R4 is $00000002",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest,
       MinuendGreaterThanSubtrahendSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000005",
        "R4 is $00000004",
        "R5 is $00000000"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $00000005",
        "R4 is $00000004",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest,
       MinuendLessThanSubtrahendClearsTheCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R4 is $00000005",
        "R5 is $00000000"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $00000004",
        "R4 is $00000005",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, OverflowNegativeFromPositiveSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R3 is $40000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, OverflowPositiveFromNegativeSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000000"
    });
    When({
        "CMP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMPP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,USR",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMPP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMPP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "CMPP R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NVIF,SVC",
        "PC is $0000100C",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
    When({
        "CMP R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $0000100C",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $2C00100F",
        "R5 is $00000000"
    });
    When({
        "CMP R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $2C00100F",
        "R5 is $00000000"
    });
}

TEST_F(CmpOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is CIF,SVC",
        "PC is $00001008",
        "R4 is $00000100",
        "R5 is $00000001"
    });
    When({
        "CMP R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is ZCIF,SVC",
        "PC is $0000100C",
        "R4 is $00000100",
        "R5 is $00000001"
    });
}