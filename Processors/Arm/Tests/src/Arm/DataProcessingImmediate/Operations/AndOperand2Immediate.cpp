#include "Arm/ArmTest.h"

class AndOperand2ImmediateTest: public ArmTest {};

TEST_F(AndOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF"
    });
    When({
        "AND R2, R3, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000088",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R2, R3, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000000",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000001",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R2, R3, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "AND R15, R3, #0xFC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00000008",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R15, R3, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00000008",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R15, R3, #0xBC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00000008",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R15, R3, #0xBC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00000008",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "ANDS R15, R3, #0xBC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCVIF,FIQ",
        "PC is $00000008",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(AndOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "AND R2, R15, #0xF000000F"
    });
    Then({
        "CYCLES is S",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $00000008"
    });
}