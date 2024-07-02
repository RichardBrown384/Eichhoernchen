#include "Arm/ArmTest.h"

class MultiplyTest: public ArmTest {};

TEST_F(MultiplyTest, TwoSmallPositiveNumbers) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000005",
        "R5 is $0000000C"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S3I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $0000003C",
        "R3 is $00000005",
        "R5 is $0000000C"
    });
}

TEST_F(MultiplyTest, TwoSmallNegativeNumbers) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $FFFFFFFF",
        "R5 is $FFFFFFFE"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S16I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000002",
        "R3 is $FFFFFFFF",
        "R5 is $FFFFFFFE"
    });
}

TEST_F(MultiplyTest, ZeroResultSetsZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000005",
        "R5 is $00000000"
    });
    When({
        "MULS R1, R3, R5"
    });
    Then({
        "CYCLES is SI",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000005",
        "R5 is $00000000"
    });
}

TEST_F(MultiplyTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000001",
        "R5 is $80000000"
    });
    When({
        "MULS R1, R3, R5"
    });
    Then({
        "CYCLES is S16I",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R1 is $80000000",
        "R3 is $00000001",
        "R5 is $80000000"
    });
}

TEST_F(MultiplyTest, OverflowFlagIsUnaffected) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000002",
        "R5 is $00000008"
    });
    When({
        "MULS R1, R3, R5"
    });
    Then({
        "CYCLES is S3I",
        "PSR is V,SVC",
        "PC is $0000100C",
        "R1 is $00000010",
        "R3 is $00000002",
        "R5 is $00000008"
    });
}

TEST_F(MultiplyTest,
       WhenTheSameRegisterOccupiesTheRdAndRmPositionsTheResultIsZero) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005"
    });
    When({
        "MUL R3, R3, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R5 is $00000005"
    });
}

TEST_F(MultiplyTest,
       UpdatingThePSRWhenTheSameRegisterOccupiesTheRdAndRmPositionsSetsTheZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005"
    });
    When({
        "MULS R3, R3, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R5 is $00000005"
    });
}

TEST_F(MultiplyTest,
       WhenR15OccupiesTheRdPositionTheProgramCounterIsUnaffected) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005"
    });
    When({
        "MUL R15, R3, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000004",
        "R5 is $00000005"
    });
}

TEST_F(MultiplyTest,
       UpdatingThePSRWhenR15OccupiesTheRdAndRmPositionsFails) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R5 is $00000005"
    });
    When({
        "MULS R15, R15, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R5 is $00000005"
    });
}

TEST_F(MultiplyTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000001"
    });
    When({
        "MUL R3, R15, R5"
    });
    Then({
        "CYCLES is SI",
        "PSR is NIF,SVC",
        "PC is $0000100C",
        "R3 is $8C00100F",
        "R5 is $00000001"
    });
}

TEST_F(MultiplyTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000001"
    });
    When({
        "MUL R3, R5, R15"
    });
    Then({
        "CYCLES is S7I",
        "PSR is NIF,SVC",
        "PC is $0000100C",
        "R3 is $00001008",
        "R5 is $00000001"
    });
}