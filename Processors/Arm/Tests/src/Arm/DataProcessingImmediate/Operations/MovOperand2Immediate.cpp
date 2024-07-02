#include "Arm/ArmTest.h"

class MovOperand2ImmediateTest: public ArmTest {};

TEST_F(MovOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MOV R2, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000088"
    });
}

TEST_F(MovOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MOVS R2, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000000"
    });
}

TEST_F(MovOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000001"
    });
    When({
        "MOVS R2, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000"
    });
}

TEST_F(MovOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008"
    });
    When({
        "MOV R15, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $00000008"
    });
}

TEST_F(MovOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008"
    });
    When({
        "MOVS R15, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NC,USR",
        "PC is $00000008"
    });
}

TEST_F(MovOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008"
    });
    When({
        "MOVS R15, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00000008"
    });
}

TEST_F(MovOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008"
    });
    When({
        "MOVS R15, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00000008"
    });
}

TEST_F(MovOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008"
    });
    When({
        "MOVS R15, #0xAC000001"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NCIF,FIQ",
        "PC is $00000008"
    });
}