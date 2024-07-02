#include "Arm/ArmTest.h"

class LoadByteImmediatePostIndexedTest: public ArmTest {};

TEST_F(LoadByteImmediatePostIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRB R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $00008020",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008001).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $00008021",
        "($00008001).B is $77",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $00007FE0",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008001).B is $77",
        "($00007EF0).B is $DD"
    });
    When({
        "LDRB R1, [R2], #-0x21"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $00007FE0",
        "($00008001).B is $77",
        "($00007EF0).B is $DD"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
       WhenTheSameImmediateOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRB R2, [R2], #0x10"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00000077",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "LDRB R15, [R2], #0x10"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "R2 is $00008010",
        "($00008000).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001008).B is $77"
    });
    When({
        "LDRB R1, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00001710",
        "R1 is $00000077",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "($00001008).B is $77"
    });
    When({
        "LDRB R15, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "($00001008).B is $77"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
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
        "LDRB R1, [R2], #0x10"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
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
        "LDRB R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadByteImmediatePostIndexedTest,
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
        "LDRB R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}