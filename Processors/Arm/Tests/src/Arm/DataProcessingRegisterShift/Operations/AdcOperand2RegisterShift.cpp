#include "Arm/ArmTest.h"

class AdcOperand2RegisterShiftTest: public ArmTest {};

TEST_F(AdcOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "ADC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000090",
        "R3 is $00000008",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "ADC R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000008",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "ADC R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000008",
        "R3 is $00000008",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "ADC R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000008",
        "R3 is $00000008",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, CarryFlagSetInfluencesResult) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "ADC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000021",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, CarryFlagClearInfluencesResult) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "ADC R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000020",
        "R3 is $00000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
    When({
        "ADCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000020",
        "R3 is $80000000",
        "R4 is $00000001",
        "R5 is $00000005"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ADCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, CarryOutSetsTheCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $C0000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $C0000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, OverflowFromPositiveNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000000",
        "R4 is $40000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, OverflowFromNegativeNumbersSetsTheOverflowFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $80000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R2, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $80000000",
        "R4 is $80000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "ADC R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCV,USR",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
    When({
        "ADCS R15, R3, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $AC002000",
        "R4 is $10000001",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ADC R2, R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $0000100C",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000000"
    });
    When({
        "ADC R2, R4, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100F",
        "R4 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(AdcOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000",
        "R5 is $00000001"
    });
    When({
        "ADC R2, R4, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00000100",
        "R4 is $00000000",
        "R5 is $00000001"
    });
}