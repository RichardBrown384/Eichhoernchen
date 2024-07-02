#include "Arm/ArmTest.h"

class StoreByteRegisterPostIndexedUserModeTest: public ArmTest {};

TEST_F(StoreByteRegisterPostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $77"
    });
    When({
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $EE"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008001).B is $77",
        "($00008021).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008001).B is $EE",
        "($00008021).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008001).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008001).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $77",
        "($00008020).B is $DD"
    });
    When({
        "STRBT R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $77",
        "($00007FE0).B is $DD"
    });
    When({
        "STRBT R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
    When({
        "STRBT R15, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).B is $0F"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPositionThenProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).B is $77",
        "($0000100C).B is $22"
    });
    When({
        "STRBT R1, [R15], R2"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).B is $DD",
        "($0000100C).B is $22"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00004000",
        "($00004000).B is $77"
    });
    When({
        "STRBT R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $AABBCCDD",
        "R2 is $000442C0",
        "($00004000).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
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
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
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
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
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

TEST_F(StoreByteRegisterPostIndexedUserModeTest,
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
        "STRBT R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
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