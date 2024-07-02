#include "Arm/ArmTest.h"

class StoreWordRegisterPostIndexedTest: public ArmTest {};

TEST_F(StoreWordRegisterPostIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $118899EE"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveOffset) {
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
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NonAlignedPositiveOffset) {
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
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeOffset) {
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
        "STR R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NonAlignedNegativeOffset) {
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
        "STR R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008000).W is $118899EE",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveLogicalShiftLeftOffset) {
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
        "STR R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeLogicalShiftLeftOffset) {
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
        "STR R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveLogicalShiftRightOffset) {
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
        "STR R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeLogicalShiftRightOffset) {
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
        "STR R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveArithmeticShiftRightOffset) {
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
        "STR R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeArithmeticShiftRightOffset) {
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
        "STR R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveRotateRightOffset) {
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
        "STR R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeRotateRightOffset) {
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
        "STR R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, PositiveRotateRightExtendOffset) {
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
        "STR R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest, NegativeRotateRightExtendOffset) {
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
        "STR R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $118899EE",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677"
    });
    When({
        "STR R15, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).W is $C800100F"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
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
        "STR R1, [R15], R2"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00001008).W is $AABBCCDD",
        "($0000100C).W is $DDEE1122"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00004000",
        "($00004000).W is $44556677"
    });
    When({
        "STR R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $000442C0",
        "($00004000).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
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
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
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
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(StoreWordRegisterPostIndexedTest,
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
        "STR R1, [R2], R3"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $07FFFFF0",
        "R3 is $00000000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}