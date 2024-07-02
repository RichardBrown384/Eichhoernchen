#include "Arm/ArmTest.h"

class MultiplyTimingTest: public ArmTest {};

TEST_F(MultiplyTimingTest,
       WhenRsIs0x0TheOperationShouldTakeOneInternalCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is SI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1TheOperationShouldTakeOneInternalCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000001"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is SI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000001"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x2TheOperationShouldTakeTwoInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000002"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000002"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7TheOperationShouldTakeTwoInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000007"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S2I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000007"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x8TheOperationShouldTakeThreeInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000008"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S3I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000008"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FTheOperationShouldTakeThreeInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $0000001F"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S3I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $0000001F"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x20TheOperationShouldTakeFourInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000020"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S4I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000020"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FTheOperationShouldTakeFourInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $0000007F"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S4I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $0000007F"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x80TheOperationShouldTakeFiveInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000080"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S5I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000080"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFTheOperationShouldTakeFiveInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $000001FF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S5I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $000001FF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x200TheOperationShouldTakeSixInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000200"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S6I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000200"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FFTheOperationShouldTakeSixInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $000007FF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S6I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $000007FF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x800TheOperationShouldTakeSevenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00000800"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S7I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000800"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFFTheOperationShouldTakeSevenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00001FFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S7I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00001FFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x2000TheOperationShouldTakeEightInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00002000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S8I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00002000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FFFTheOperationShouldTakeEightInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00007FFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S8I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00007FFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x8000TheOperationShouldTakeNineInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00008000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S9I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00008000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFFFTheOperationShouldTakeNineInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $0001FFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S9I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $0001FFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x20000TheOperationShouldTakeTenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00020000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S10I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00020000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FFFFTheOperationShouldTakeTenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $0007FFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S10I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $0007FFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x80000TheOperationShouldTakeElevenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00080000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S11I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00080000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFFFFTheOperationShouldTakeElevenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $001FFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S11I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $001FFFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x200000TheOperationShouldTakeTwelveInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00200000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S12I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00200000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FFFFFTheOperationShouldTakeTwelveInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $007FFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S12I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $007FFFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x800000TheOperationShouldTakeThirteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $00800000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S13I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00800000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFFFFFTheOperationShouldTakeThirteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $01FFFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S13I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $01FFFFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x2000000TheOperationShouldTakeFourteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $02000000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S14I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $02000000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x7FFFFFFTheOperationShouldTakeFourteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $07FFFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S14I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $07FFFFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x8000000TheOperationShouldTakeFifteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $08000000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S15I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $08000000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x1FFFFFFFTheOperationShouldTakeFifteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $1FFFFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S15I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $1FFFFFFF"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0x20000000TheOperationShouldTakeSixteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $20000000"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S16I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $20000000"
    });
}

TEST_F(MultiplyTimingTest,
       WhenRsIs0xFFFFFFFFTheOperationShouldTakeSixteenInternalCycles) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R3 is $00000000",
        "R5 is $FFFFFFFF"
    });
    When({
        "MUL R1, R3, R5"
    });
    Then({
        "CYCLES is S16I",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $FFFFFFFF"
    });
}