#include "Arm/ArmTest.h"

class StoreWordImmediatePreIndexedWritebackTest: public ArmTest {};

TEST_F(StoreWordImmediatePreIndexedWritebackTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $AABBCCDD"
    });
    When({
        "STR R1, [R2]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $55667788"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $11223344",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STR R1, [R2, #0x20]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008020",
        "($00008000).W is $11223344",
        "($00008020).W is $55667788"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $11223344",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STR R1, [R2, #0x21]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008021",
        "($00008000).W is $11223344",
        "($00008020).W is $55667788"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $11223344",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STR R1, [R2, #-0x20]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00007FE0",
        "($00008000).W is $11223344",
        "($00007FE0).W is $55667788"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).W is $11223344",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STR R1, [R2, #-0x21]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00007FDF",
        "($00008000).W is $11223344",
        "($00007FDC).W is $55667788"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008010).W is $44556677"
    });
    When({
        "STR R15, [R2, #0x10]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008010",
        "($00008010).W is $C800100F"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPositionThenProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001708).W is $44556677",
        "($0000170C).W is $DDEE1122"
    });
    When({
        "STR R1, [R15, #0x700]!"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $0001710",
        "R1 is $AABBCCDD",
        "($00001708).W is $AABBCCDD",
        "($0000170C).W is $DDEE1122"
    });
}

TEST_F(StoreWordImmediatePreIndexedWritebackTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRangeAndTheBaseRegisterIsNotUpdated) {
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
        "STR R1, [R2, #0x10]!"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreWordImmediatePreIndexedWritebackTest,
       DataAbortExceptionIsTakenWhenTheMemoryManagementSystemSignalsAbortAndTheBaseRegisterIsNotUpdated) {
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
        "STR R1, [R2, #0x0]!"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreWordImmediatePreIndexedWritebackTest,
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
        "STR R1, [R2, #0x110]!"
    });
    Then({
        "CYCLES is NNNS",
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