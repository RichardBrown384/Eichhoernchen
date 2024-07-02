#include "Arm/ArmTest.h"

class AndOperand2InstructionShiftTest: public ArmTest {};

TEST_F(AndOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF",
        "R4 is $00000011"
    });
    When({
        "AND R2, R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000088",
        "R3 is $FFFFFFFF",
        "R4 is $00000011"
    });
}

TEST_F(AndOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
    When({
        "AND R2, R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(AndOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
    When({
        "AND R2, R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $E0000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(AndOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000001"
    });
    When({
        "AND R2, R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000000",
        "R3 is $FFFFFFFF",
        "R4 is $80000001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $80001010",
        "R4 is $00000021"
    });
    When({
        "AND R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $80000010",
        "R3 is $80001010",
        "R4 is $00000021"
    });
}

TEST_F(AndOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF",
        "R4 is $00000001"
    });
    When({
        "ANDS R2, R3, R4, LSL #31"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000000",
        "R3 is $FFFFFFFF",
        "R4 is $00000001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A"
    });
    When({
        "ANDS R2, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A"
    });
}

TEST_F(AndOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $FC002001"
    });
    When({
        "AND R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R3 is $FFFFFFFF",
        "R4 is $FC002001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001"
    });
    When({
        "ANDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "ANDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "ANDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(AndOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "ANDS R15, R3, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00002008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(AndOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $FFFFFFFF"
    });
    When({
        "AND R2, R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00001008",
        "R4 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $FFFFFFFF"
    });
    When({
        "AND R2, R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $1C00100B",
        "R4 is $FFFFFFFF"
    });
}