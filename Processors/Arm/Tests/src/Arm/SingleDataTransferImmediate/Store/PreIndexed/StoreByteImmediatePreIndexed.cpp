#include "Arm/ArmTest.h"

class StoreByteImmediatePreIndexedTest: public ArmTest {};

TEST_F(StoreByteImmediatePreIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $DD"
    });
    When({
        "STRB R1, [R2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $88"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "STRB R1, [R2, #0x20]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008020).B is $88"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008021).B is $DD"
    });
    When({
        "STRB R1, [R2, #0x21]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008021).B is $88"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "STRB R1, [R2, #-0x20]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00007FE0).B is $88"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "STRB R1, [R2, #-0x21]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $55667788",
        "R2 is $00008000",
        "($00008000).B is $44",
        "($00007FDF).B is $88"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "STRB R15, [R2, #0x0]"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008000",
        "($00008000).B is $0F"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001708).B is $77",
        "($0000170C).B is $22"
    });
    When({
        "STRB R1, [R15, #0x700]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "($00001708).B is $DD",
        "($0000170C).B is $22"
    });
}

TEST_F(StoreByteImmediatePreIndexedTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRange) {
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
        "STRB R1, [R2, #0x10]"
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

TEST_F(StoreByteImmediatePreIndexedTest,
       DataAbortExceptionIsTakenWhenTheMemoryManagementSystemSignalsAbort) {
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
        "STRB R1, [R2, #0x0]"
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

TEST_F(StoreByteImmediatePreIndexedTest,
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
        "STRB R1, [R2, #0x110]"
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