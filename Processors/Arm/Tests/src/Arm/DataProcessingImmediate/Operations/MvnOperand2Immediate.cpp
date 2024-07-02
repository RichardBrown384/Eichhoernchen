#include "Arm/ArmTest.h"

class MvnOperand2ImmediateTest: public ArmTest {};

TEST_F(MvnOperand2ImmediateTest, Immediate) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MVN R2, #0x88"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFF77"
    });
}

TEST_F(MvnOperand2ImmediateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000"
    });
    When({
        "MVNS R2, #0"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF"
    });
}

TEST_F(MvnOperand2ImmediateTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008"
    });
    When({
        "MVN R15, #0x00FF0000"
    });
    Then({
        "CYCLES is SNS",
        "PSR is 0,USR",
        "PC is $03010004"
    });
}

TEST_F(MvnOperand2ImmediateTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008"
    });
    When({
        "MVNS R15, #0x00FF0000"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCV,USR",
        "PC is $03010004"
    });
}

TEST_F(MvnOperand2ImmediateTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008"
    });
    When({
        "MVNS R15, #0x00FF0000"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCVIF,SVC",
        "PC is $03010004"
    });
}

TEST_F(MvnOperand2ImmediateTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008"
    });
    When({
        "MVNS R15, #0x00FF0000"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCVIF,SVC",
        "PC is $03010004"
    });
}

TEST_F(MvnOperand2ImmediateTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008"
    });
    When({
        "MVNS R15, #0x00FF0000"
    });
    Then({
        "CYCLES is SNS",
        "PSR is NZCVIF,SVC",
        "PC is $03010004"
    });
}