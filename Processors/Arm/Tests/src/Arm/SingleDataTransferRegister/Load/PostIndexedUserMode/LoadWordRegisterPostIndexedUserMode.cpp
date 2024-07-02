#include "Arm/ArmTest.h"

class LoadWordRegisterPostIndexedUserModeTest: public ArmTest {};

TEST_F(LoadWordRegisterPostIndexedUserModeTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $77445566",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008001",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $77445566",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
    When({
        "LDRT R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
       WhenTheSameRegisterOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R2, [R2], R3"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R2 is $44556677",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R15, [R2], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "SPMD is SsSS,S",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).W is $44556677"
    });
    When({
        "LDRT R1, [R15], R2"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "SPMD is SsSS,S",
        "R1 is $44556677",
        "R2 is $00007000",
        "($00001008).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "($00008000).W is $44556677"
    });
    When({
        "LDRT R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is SNI",
        "PSR is F,SVC",
        "PC is $0000100C",
        "SPMD is Ss,S", 
        "R1 is $44556677",
        "R2 is $000482C0",
        "($00008000).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R3 is $00007000",
        "($00001008).W is $44556677"
    });
    When({
        "LDRT R15, [R15], R3"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0055667C",
        "SPMD is SsSS,S",
        "R3 is $00007000",
        "($00001008).W is $44556677"
    });
}

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
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
        "LDRT R1, [R2], R3"
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

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
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
        "LDRT R1, [R2], R3"
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

TEST_F(LoadWordRegisterPostIndexedUserModeTest,
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
        "LDRT R1, [R2], R3"
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