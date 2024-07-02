#include "Arm/ArmTest.h"

class LoadByteImmediatePostIndexedUserModeTest: public ArmTest {};

TEST_F(LoadByteImmediatePostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $44"
    });
    When({
        "LDRBT R1, [R2], #0"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008000",
        "($00008000).B is $44"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRBT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008001).B is $44",
        "($00008021).B is $DD"
    });
    When({
        "LDRBT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00008021",
        "($00008001).B is $44",
        "($00008021).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRBT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE0",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008001).B is $44",
        "($00007FDF).B is $DD"
    });
    When({
        "LDRBT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $00000044",
        "R2 is $00007FE1",
        "($00008001).B is $44",
        "($00007FDF).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       WhenTheSameImmediateOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRBT R2, [R2], #0x10"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R2 is $00000077",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRBT R15, [R2], #0x10"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "SPMD is SsSS,S",
        "R2 is $00008010",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001008).B is $77"
    });
    When({
        "LDRBT R1, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00001710",
        "SPMD is SsSS,S",
        "R1 is $00000077",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "($00001008).B is $77"
    });
    When({
        "LDRBT R15, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "SPMD is SsSS,S",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRangeAndTheBaseImmediateIsNotUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "LDRBT R1, [R2], #0x10"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       DataAbortExceptionIsTakenWhenTheMemoryManagementSystemSignalsAbortAndTheBaseImmediateIsNotUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00008000).W is ABORT"
    });
    When({
        "LDRBT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteImmediatePostIndexedUserModeTest,
       AddressExceptionsTakePrecedenceOverDataAborts) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($03FFFFF0).W is ABORT"
    });
    When({
        "LDRBT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}