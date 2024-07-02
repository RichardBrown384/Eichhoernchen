#include "Arm/ArmTest.h"

class TstOperand2ImmediateTest: public ArmTest {};

TEST_F(TstOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "TST R3, #0x1"
    });
    Then({
        "CYCLES is S",
        "PSR is V,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(TstOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "TST R3, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(TstOperand2ImmediateTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $FFFFFFFF"
    });
    When({
        "TST R3, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $FFFFFFFF"
    });
}

TEST_F(TstOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R3 is $AC002001"
    });
    When({
        "TSTP R3, #0xFC000003"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,USR",
        "PC is $0000100C",
        "R3 is $AC002001"
    });
}

TEST_F(TstOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R3 is $BC002001"
    });
    When({
        "TSTP R3, #0xFC000003"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $BC002001"
    });
}

TEST_F(TstOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R3 is $BC002001"
    });
    When({
        "TSTP R3, #0xFC000003"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $BC002001"
    });
}

TEST_F(TstOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $BC002001"
    });
    When({
        "TSTP R3, #0xFC000003"
    });
    Then({
        "CYCLES is S",
        "PSR is NCVIF,FIQ",
        "PC is $0000100C",
        "R3 is $BC002001"
    });
}

TEST_F(TstOperand2ImmediateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008"
    });
    When({
        "TST R15, #0xF7"
    });
    Then({
        "CYCLES is S",
        "PSR is ZVIF,SVC",
        "PC is $0000100C"
    });
}