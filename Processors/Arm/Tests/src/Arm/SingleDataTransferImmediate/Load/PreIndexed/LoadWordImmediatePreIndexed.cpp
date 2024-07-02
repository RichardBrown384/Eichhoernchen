#include "Arm/ArmTest.h"

class LoadWordImmediatePreIndexedTest: public ArmTest {};

TEST_F(LoadWordImmediatePreIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
    When({
        "LDR R1, [R2]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $44556677",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDR R1, [R2, #0x20]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDR R1, [R2, #0x21]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $DDAABBCC",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDR R1, [R2, #-0x20]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
    When({
        "LDR R1, [R2, #-0x21]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $BBCCDDAA",
        "R2 is $00008000",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
       WhenTheSameImmediateOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008010).W is $44556677"
    });
    When({
        "LDR R2, [R2, #0x10]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $44556677",
        "($00008010).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008010).W is $44556677"
    });
    When({
        "LDR R15, [R2, #0x10]"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "R2 is $00008000",
        "($00008010).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
       WhenR15IsInRnPositionTheProgramCounterIsNotUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001708).W is $44556677",
        "($0000170C).W is $DDEE1122"
    });
    When({
        "LDR R1, [R15, #0x700]"
    });
    Then({
        "CYCLES is SNI",
        "PSR is ZIF,SVC",
        "PC is $0000100C",
        "R1 is $44556677",
        "($00001708).W is $44556677",
        "($0000170C).W is $DDEE1122"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "($00001708).W is $44556677"
    });
    When({
        "LDR R15, [R15, #0x700]"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "($00001708).W is $44556677"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRangeAndTheBaseImmediateIsNotUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "LDR R1, [R2, #0x10]"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(LoadWordImmediatePreIndexedTest,
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
        "LDR R1, [R2, #0x0]"
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

TEST_F(LoadWordImmediatePreIndexedTest,
       AddressExceptionsTakePrecedenceOverDataAborts) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00000100).W is ABORT"
    });
    When({
        "LDR R1, [R2, #0x110]"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}