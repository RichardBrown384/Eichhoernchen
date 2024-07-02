#include "Arm/ArmTest.h"

class TeqOperand2ImmediateTest: public ArmTest {};

TEST_F(TeqOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00001080"
    });
    When({
        "TEQ R3, #0x1080"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00001080"
    });
}

TEST_F(TeqOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $40000001"
    });
    When({
        "TEQ R3, #0xC0000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $40000001"
    });
}

TEST_F(TeqOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00FF0000"
    });
    When({
        "TEQ R3, #0x00FF0000"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00FF0000"
    });
}

TEST_F(TeqOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R3 is $FC002001"
    });
    When({
        "TEQP R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCV,USR",
        "PC is $0000100C",
        "R3 is $FC002001"
    });
}

TEST_F(TeqOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is N,IRQ",
        "PC is $00001008",
        "R3 is $FC002003"
    });
    When({
        "TEQP R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,IRQ",
        "PC is $0000100C",
        "R3 is $FC002003"
    });
}

TEST_F(TeqOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is V,FIQ",
        "PC is $00001008",
        "R3 is $FC002002"
    });
    When({
        "TEQP R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,SVC",
        "PC is $0000100C",
        "R3 is $FC002002"
    });
}

TEST_F(TeqOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $FC002001"
    });
    When({
        "TEQP R3, #0x40000001"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,USR",
        "PC is $0000100C",
        "R3 is $FC002001"
    });
}

TEST_F(TeqOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00000078"
    });
    When({
        "TEQ R15, #0x78"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000007C"
    });
}