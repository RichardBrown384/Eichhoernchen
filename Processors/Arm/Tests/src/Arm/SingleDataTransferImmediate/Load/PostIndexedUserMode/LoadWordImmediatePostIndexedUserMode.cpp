#include "Arm/ArmTest.h"

class LoadWordImmediatePostIndexedUserModeTest: public ArmTest {};

TEST_F(LoadWordImmediatePostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $77445566",
        "R2 is $00008021",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $77445566",
        "R2 is $00007FE1",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
       WhenTheSameImmediateOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R2, [R2], #0x10"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R2 is $44556677",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R15, [R2], #0x10"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "SPMD is SsSS,S",
        "R2 is $00008010",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001008).W is $44556677"
    });
    When({
        "LDRT R1, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00001710",
        "SPMD is SsSS,S",
        "R1 is $44556677",
        "($00001008).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "($00001008).W is $44556677"
    });
    When({
        "LDRT R15, [R15], #0x700"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "SPMD is SsSS,S",
        "($00001008).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
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
        "LDRT R1, [R2], #0x10"
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

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
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
        "LDRT R1, [R2], #0x0"
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

TEST_F(LoadWordImmediatePostIndexedUserModeTest,
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
        "LDRT R1, [R2], #0x0"
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