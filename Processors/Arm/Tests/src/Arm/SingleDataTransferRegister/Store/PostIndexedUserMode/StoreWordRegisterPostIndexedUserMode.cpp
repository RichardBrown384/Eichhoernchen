#include "Arm/ArmTest.h"

class StoreWordRegisterPostIndexedUserModeTest: public ArmTest {};

TEST_F(StoreWordRegisterPostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "STRT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $118899EE"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "STRT R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
    When({
        "STRT R15, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).W is $C800100F"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPositionThenProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).W is $44556677",
        "($0000100C).W is $DDEE1122"
    });
    When({
        "STRT R1, [R15], R2"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "SPMD is SsSS,S",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).W is $AABBCCDD",
        "($0000100C).W is $DDEE1122"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00004000",
        "($00004000).W is $44556677"
    });
    When({
        "STRT R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S",
        "R1 is $AABBCCDD",
        "R2 is $000442C0",
        "($00004000).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
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
        "STRT R1, [R2], R3"
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

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
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
        "STRT R1, [R2], R3"
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

TEST_F(StoreWordRegisterPostIndexedUserModeTest,
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
        "STRT R1, [R2], R3"
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