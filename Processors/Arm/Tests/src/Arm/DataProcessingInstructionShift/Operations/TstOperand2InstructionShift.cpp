#include "Arm/ArmTest.h"

class TstOperand2InstructionShiftTest: public ArmTest {};

TEST_F(TstOperand2InstructionShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $10000011"
    });
    When({
        "TST R3, R4, LSL #3"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $10000011"
    });
}

TEST_F(TstOperand2InstructionShiftTest, LogicalShiftRight) {
    Given({
        "PSR is NZ,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
    When({
        "TST R3, R4, LSR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(TstOperand2InstructionShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
    When({
        "TST R3, R4, ASR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000000"
    });
}

TEST_F(TstOperand2InstructionShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000007"
    });
    When({
        "TST R3, R4, ROR #2"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000007"
    });
}

TEST_F(TstOperand2InstructionShiftTest, RotateRightWithExtend) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $80000007"
    });
    When({
        "TST R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $80000007"
    });
}

TEST_F(TstOperand2InstructionShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $00000001"
    });
    When({
        "TST R3, R4, LSL #31"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $00000001"
    });
}

TEST_F(TstOperand2InstructionShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A"
    });
    When({
        "TST R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $A5A5A5A5",
        "R4 is $5A5A5A5A"
    });
}

TEST_F(TstOperand2InstructionShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001"
    });
    When({
        "TSTP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,USR",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $AC002001"
    });
}

TEST_F(TstOperand2InstructionShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "TSTP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(TstOperand2InstructionShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "TSTP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(TstOperand2InstructionShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
    When({
        "TSTP R3, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $FFFFFFFF",
        "R4 is $BC002001"
    });
}

TEST_F(TstOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $FFFFEFF3"
    });
    When({
        "TST R15, R4, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $FFFFEFF3"
    });
}

TEST_F(TstOperand2InstructionShiftTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R4 is $E3FFEFF0"
    });
    When({
        "TST R4, R15, LSL #0"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000100C",
        "R4 is $E3FFEFF0"
    });
}