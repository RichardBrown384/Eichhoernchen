#include "Arm/ArmTest.h"

class MultiplyAccumulateTest: public ArmTest {};

TEST_F(MultiplyAccumulateTest, TwoSmallPositiveNumbers) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000005",
        "R5 is $0000000C",
        "R6 is $00000002"
    });
    When({
        "MLA R1, R3, R5, R6"
    });
    Then({
        "CYCLES is S3I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $0000003E",
        "R3 is $00000005",
        "R5 is $0000000C",
        "R6 is $00000002"
    });
}

TEST_F(MultiplyAccumulateTest, TwoSmallNegativeNumbers) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $FFFFFFFF",
        "R5 is $FFFFFFFE",
        "R6 is $00000003"
    });
    When({
        "MLA R1, R3, R5, R6"
    });
    Then({
        "CYCLES is S16I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000005",
        "R3 is $FFFFFFFF",
        "R5 is $FFFFFFFE",
        "R6 is $00000003"
    });
}

TEST_F(MultiplyAccumulateTest, ZeroResultSetsZeroFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $FFFFFFFF",
        "R5 is $00000001",
        "R6 is $00000001"
    });
    When({
        "MLAS R1, R3, R5, R6"
    });
    Then({
        "CYCLES is SI",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $FFFFFFFF",
        "R5 is $00000001",
        "R6 is $00000001"
    });
}

TEST_F(MultiplyAccumulateTest, NegativeResultSetsNegativeFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000001",
        "R5 is $80000000",
        "R6 is $00000001"
    });
    When({
        "MLAS R1, R3, R5, R6"
    });
    Then({
        "CYCLES is S16I",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R1 is $80000001",
        "R3 is $00000001",
        "R5 is $80000000",
        "R6 is $00000001"
    });
}

TEST_F(MultiplyAccumulateTest, OverflowFlagIsUnaffected) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000002",
        "R5 is $00000008",
        "R6 is $00000003"
    });
    When({
        "MLAS R1, R3, R5, R6"
    });
    Then({
        "CYCLES is S3I",
        "PSR is V,SVC",
        "PC is $0000100C",
        "R1 is $00000013",
        "R3 is $00000002",
        "R5 is $00000008",
        "R6 is $00000003"
    });
}

TEST_F(MultiplyAccumulateTest,
       WhenTheSameRegisterOccupiesTheRdAndRmPositionsTheResultIsRn) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005",
        "R6 is $0000003E"
    });
    When({
        "MLA R3, R3, R5, R6"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $0000003E",
        "R5 is $00000005",
        "R6 is $0000003E"
    });
}

TEST_F(MultiplyAccumulateTest,
       UpdatingThePSRWhenTheSameRegisterOccupiesTheRdAndRmPositionsSetsTheZeroFlagIfRnIsZero) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005",
        "R6 is $00000000"
    });
    When({
        "MLAS R3, R3, R5, R6"
    });
    Then({
        "CYCLES is S2I",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R3 is $00000000",
        "R5 is $00000005",
        "R6 is $00000000"
    });
}

TEST_F(MultiplyAccumulateTest,
       UpdatingThePSRWhenTheSameRegisterOccupiesTheRdAndRmPositionsClearsTheZeroFlagIfRnIsNonZero) {
    Given({
        "PSR is Z,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005",
        "R6 is $00000003"
    });
    When({
        "MLAS R3, R3, R5, R6"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000003",
        "R5 is $00000005",
        "R6 is $00000003"
    });
}

TEST_F(MultiplyAccumulateTest,
       WhenR15OccupiesTheRdPositionTheProgramCounterIsUnaffected) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000005",
        "R6 is $00000010"
    });
    When({
        "MLA R15, R3, R5, R6"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R3 is $00000004",
        "R5 is $00000005",
        "R6 is $00000010"
    });
}

TEST_F(MultiplyAccumulateTest,
       UpdatingThePSRWhenR15OccupiesTheRdAndRmPositionsFails) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R5 is $00000005",
        "R6 is $00000000"
    });
    When({
        "MLAS R15, R15, R5, R6"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R5 is $00000005",
        "R6 is $00000000"
    });
}

TEST_F(MultiplyAccumulateTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000001",
        "R6 is $00000000"
    });
    When({
        "MLA R3, R15, R5, R6"
    });
    Then({
        "CYCLES is SI",
        "PSR is NIF,SVC",
        "PC is $0000100C",
        "R3 is $8C00100F",
        "R5 is $00000001",
        "R6 is $00000000"
    });
}

TEST_F(MultiplyAccumulateTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRsPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000001",
        "R6 is $00000000"
    });
    When({
        "MLA R3, R5, R15, R6"
    });
    Then({
        "CYCLES is S7I",
        "PSR is NIF,SVC",
        "PC is $0000100C",
        "R3 is $00001008",
        "R5 is $00000001",
        "R6 is $00000000"
    });
}

TEST_F(MultiplyAccumulateTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is NIF,SVC",
        "PC is $00001008",
        "R3 is $00000004",
        "R5 is $00000000",
        "R6 is $00000000"
    });
    When({
        "MLA R3, R5, R6, R15"
    });
    Then({
        "CYCLES is SI",
        "PSR is NIF,SVC",
        "PC is $0000100C",
        "R3 is $8C00100B",
        "R5 is $00000000",
        "R6 is $00000000"
    });
}
