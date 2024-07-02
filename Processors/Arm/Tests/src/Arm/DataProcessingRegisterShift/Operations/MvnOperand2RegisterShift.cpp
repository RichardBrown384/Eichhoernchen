#include "Arm/ArmTest.h"

class MvnOperand2RegisterShiftTest: public ArmTest {};

TEST_F(MvnOperand2RegisterShiftTest, LogicalShiftLeft) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $00000011",
        "R5 is $00000003"
    });
    When({
        "MVN R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFF77",
        "R4 is $00000011",
        "R5 is $00000003"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, LogicalShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "MVN R2, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $DFFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, ArithmeticShiftRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000000",
        "R5 is $00000002"
    });
    When({
        "MVN R2, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $1FFFFFFF",
        "R4 is $80000000",
        "R5 is $00000002"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, RotateRight) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $80000001",
        "R5 is $00000002"
    });
    When({
        "MVN R2, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $9FFFFFFF",
        "R4 is $80000001",
        "R5 is $00000002"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R4 is $3FFFFFFF",
        "R5 is $00000001"
    });
    When({
        "MVNS R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R4 is $3FFFFFFF",
        "R5 is $00000001"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, ZeroResultSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000000",
        "R4 is $FFFFFFFF",
        "R5 is $00000000"
    });
    When({
        "MVNS R2, R4, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R2 is $00000000",
        "R4 is $FFFFFFFF",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, R15DestinationPreservePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
    When({
        "MVN R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is 0,USR",
        "PC is $00002008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, R15DestinationUpdatePSRUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
    When({
        "MVNS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NC,USR",
        "PC is $00002008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, R15DestinationUpdatePSRInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
    When({
        "MVNS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, R15DestinationUpdatePSRFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
    When({
        "MVNS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest, R15DestinationUpdatePSRSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
    When({
        "MVNS R15, R4, LSL R5"
    });
    Then({
        "CYCLES is SSNS",
        "PSR is NCIF,FIQ",
        "PC is $00002008",
        "R4 is $53FFDFFE",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R5 is $00000000"
    });
    When({
        "MVN R2, R15, LSL R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $E3FFEFF0",
        "R5 is $00000000"
    });
}

TEST_F(MvnOperand2RegisterShiftTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is VIF,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R5 is $00000001"
    });
    When({
        "MVN R2, R5, LSL R15"
    });
    Then({
        "CYCLES is SS",
        "PSR is VIF,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFEFF",
        "R5 is $00000001"
    });
}