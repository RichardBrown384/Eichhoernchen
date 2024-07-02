#include "Arm/ArmTest.h"

class EorOperand2InstructionShiftTest: public ArmTest {};

TEST_F(EorOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001080",
        "R4 is $00000011"
    });
    When({
        "EOR R2, R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R3 is $00001080",
        "R4 is $00000011"
    });
}

TEST_F(EorOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000004"
    });
    When({
        "EOR R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000000",
        "R3 is $00000001",
        "R4 is $80000004"
    });
}

TEST_F(EorOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000004"
    });
    When({
        "EOR R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000000",
        "R3 is $00000001",
        "R4 is $80000004"
    });
}

TEST_F(EorOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000005"
    });
    When({
        "EOR R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R3 is $00000001",
        "R4 is $80000005"
    });
}

TEST_F(EorOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00001010",
        "R4 is $00000021"
    });
    When({
        "EOR R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $80001000",
        "R3 is $00001010",
        "R4 is $00000021"
    });
}

TEST_F(EorOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $40000001",
        "R4 is $00000003"
    });
    When({
        "EORS R2, R3, R4, LSL #30"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $40000001",
        "R4 is $00000003"
    });
}

TEST_F(EorOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $00FF5500",
        "R4 is $00FF5500"
    });
    When({
        "EORS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $00FF5500",
        "R4 is $00FF5500"
    });
}

TEST_F(EorOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
    When({
        "EOR R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is N,USR",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
}

TEST_F(EorOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $AC002001"
    });
    When({
        "EORS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $AC002001"
    });
}

TEST_F(EorOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
    When({
        "EORS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $FC002001"
    });
}

TEST_F(EorOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
    When({
        "EORS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
}

TEST_F(EorOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
    When({
        "EORS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $00000000",
        "R4 is $BC002001"
    });
}

TEST_F(EorOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000"
    });
    When({
        "EOR R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R4 is $00000000"
    });
}

TEST_F(EorOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000000"
    });
    When({
        "EOR R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100B",
        "R4 is $00000000"
    });
}