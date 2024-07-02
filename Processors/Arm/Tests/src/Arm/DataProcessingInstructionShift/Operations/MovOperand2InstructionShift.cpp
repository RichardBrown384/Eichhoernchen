#include "Arm/ArmTest.h"

class MovOperand2InstructionShiftTest: public ArmTest {};

TEST_F(MovOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000011"
    });
    When({
        "MOV R2, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000088",
        "R4 is $00000011"
    });
}

TEST_F(MovOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000"
    });
    When({
        "MOV R2, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000000",
        "R4 is $80000000"
    });
}

TEST_F(MovOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000"
    });
    When({
        "MOV R2, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000000",
        "R4 is $80000000"
    });
}

TEST_F(MovOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000001"
    });
    When({
        "MOV R2, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R4 is $80000001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80008001"
    });
    When({
        "MOV R2, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $40004000",
        "R4 is $80008001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000001"
    });
    When({
        "MOVS R2, R4, LSL #31"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000000",
        "R4 is $00000001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $00000000"
    });
    When({
        "MOVS R2, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $00000000"
    });
}

TEST_F(MovOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $AC002001"
    });
    When({
        "MOV R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R4 is $AC002001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $AC002001"
    });
    When({
        "MOVS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R4 is $AC002001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R4 is $AC002001"
    });
    When({
        "MOVS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R4 is $AC002001"
    });
    When({
        "MOVS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001"
    });
}

TEST_F(MovOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R4 is $AC002001"
    });
    When({
        "MOVS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $AC002001"
    });
}

TEST_F(MovOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MOV R2, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100B"
    });
}