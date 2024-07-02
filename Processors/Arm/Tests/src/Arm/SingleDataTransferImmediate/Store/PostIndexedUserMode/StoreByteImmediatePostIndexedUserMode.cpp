#include "Arm/ArmTest.h"

class StoreByteImmediatePostIndexedUserModeTest: public ArmTest {};

TEST_F(StoreByteImmediatePostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "STRBT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008000",
        "($00008000).B is $EE"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "($00008001).B is $77",
        "($00008021).B is $DD"
    });
    When({
        "STRBT R1, [R2], #0x20"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008021",
        "($00008001).B is $EE",
        "($00008021).B is $DD"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "($00008001).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], #-0x20"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE1",
        "($00008001).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "($00008000).B is $77"
    });
    When({
        "STRBT R15, [R2], #0x10"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R2 is $00008010",
        "($00008000).B is $0F"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPositionThenProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "($00001008).B is $77",
        "($0000100C).B is $22"
    });
    When({
        "STRBT R1, [R15], #0x700"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $00001710",
        "R1 is $AABBCCDD",
        "($00001008).B is $DD",
        "($0000100C).B is $22"
    });
}

TEST_F(StoreByteImmediatePostIndexedUserModeTest,
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
        "STRBT R1, [R2], #0x10"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreByteImmediatePostIndexedUserModeTest,
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
        "STRBT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreByteImmediatePostIndexedUserModeTest,
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
        "STRBT R1, [R2], #0x0"
    });
    Then({
        "CYCLES is NNNS",
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