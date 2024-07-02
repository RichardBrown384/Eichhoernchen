#include "Arm/ArmTest.h"

class LoadByteRegisterPostIndexedUserModeTest: public ArmTest {};

TEST_F(LoadByteRegisterPostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $44"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $44"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008001).B is $44",
        "($00008021).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008001).B is $44",
        "($00008021).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008001).B is $44",
        "($00007FDF).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008001).B is $44",
        "($00007FDF).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       WhenTheSameRegisterOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
    When({
        "LDRBT R2, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R2 is $00000077",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
    When({
        "LDRBT R15, [R2], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "SPMD is SsSS,S",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).B is $77"
    });
    When({
        "LDRBT R1, [R15], R2"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "SPMD is SsSS,S",
        "R1 is $00000077",
        "R2 is $00007000",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRBT R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is SNI",
        "PSR is F,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000077",
        "R2 is $000482C0",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R3 is $00007000",
        "($00001008).B is $77"
    });
    When({
        "LDRBT R15, [R15], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "SPMD is SsSS,S",
        "R3 is $00007000",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRangeAndTheBaseRegisterIsNotUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       DataAbortExceptionIsTakenWhenTheMemoryManagementSystemSignalsAbortAndTheBaseRegisterIsNotUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00008000).W is ABORT"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteRegisterPostIndexedUserModeTest,
       AddressExceptionsTakePrecedenceOverDataAborts) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($03FFFFF0).W is ABORT"
    });
    When({
        "LDRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}