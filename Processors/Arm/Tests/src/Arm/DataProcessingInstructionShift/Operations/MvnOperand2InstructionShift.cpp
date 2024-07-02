#include "Arm/ArmTest.h"

class MvnOperand2InstructionShiftTest: public ArmTest {};

TEST_F(MvnOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000011"
    });
    When({
        "MVN R2, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFF77",
        "R4 is $00000011"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000"
    });
    When({
        "MVN R2, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $DFFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000"
    });
    When({
        "MVN R2, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $1FFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000001"
    });
    When({
        "MVN R2, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $9FFFFFFF",
        "R4 is $80000001"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80008001"
    });
    When({
        "MVN R2, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $BFFFBFFF",
        "R4 is $80008001"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $3FFFFFFF"
    });
    When({
        "MVNS R2, R4, LSL #1"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R4 is $3FFFFFFF"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FFFFFFFF"
    });
    When({
        "MVNS R2, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $FFFFFFFF"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $53FFDFFE"
    });
    When({
        "MVN R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R4 is $53FFDFFE"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $53FFDFFE"
    });
    When({
        "MVNS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R4 is $53FFDFFE"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R4 is $53FFDFFE"
    });
    When({
        "MVNS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R4 is $53FFDFFE"
    });
    When({
        "MVNS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE"
    });
}

TEST_F(MvnOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R4 is $53FFDFFE"
    });
    When({
        "MVNS R15, R4, LSL #0"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE"
    });
}

TEST_F(MvnOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MVN R2, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $E3FFEFF4"
    });
}