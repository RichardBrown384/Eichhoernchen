#include "Arm/ArmTest.h"

class StoreWordRegisterPreIndexedTest: public ArmTest {};

TEST_F(StoreWordRegisterPreIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "STR R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00007FDC).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3, LSL #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3, LSL #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3, LSR #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3, LSR #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3, ASR #2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3, ASR #2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3, ROR #30]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3, ROR #30]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, R3, RRX]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $DDEE1122"
    });
    When({
        "STR R1, [R2, -R3, RRX]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "STR R15, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $C800100F"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).W is $44556677",
        "($0000800C).W is $DDEE1122"
    });
    When({
        "STR R1, [R15, R2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).W is $AABBCCDD",
        "($0000800C).W is $DDEE1122"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).W is $44556677"
    });
    When({
        "STR R1, [R2, R15, LSL #6]"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRange) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R3 is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "STR R1, [R2, R3]"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R3 is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}

TEST_F(StoreWordRegisterPreIndexedTest,
       DataAbortExceptionIsTakenWhenTheMemoryManagementSystemSignalsAbort) {
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
        "STR R1, [R2, R3]"
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

TEST_F(StoreWordRegisterPreIndexedTest,
       AddressExceptionsTakePrecedenceOverDataAborts) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R3 is $00001110",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00001100).W is ABORT"
    });
    When({
        "STR R1, [R2, R3]"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $AABBCCDD",
        "R2 is $03FFFFF0",
        "R3 is $00001110",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100B"
    });
}