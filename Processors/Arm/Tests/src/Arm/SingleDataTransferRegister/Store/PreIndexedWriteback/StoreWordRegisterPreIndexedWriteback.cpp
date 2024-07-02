#include "Arm/ArmTest.h"

class StoreWordRegisterPreIndexedWritebackTest: public ArmTest {};

TEST_F(StoreWordRegisterPreIndexedWritebackTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).W is $44556677"
    });
    When({
        "STR R1, [R2, R3]!"
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

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveOffset) {
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
        "STR R1, [R2, R3]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NonAlignedPositiveOffset) {
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
        "STR R1, [R2, R3]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008021",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeOffset) {
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
        "STR R1, [R2, -R3]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NonAlignedNegativeOffset) {
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
        "STR R1, [R2, -R3]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FDF",
        "R3 is $00000021",
        "($00008000).W is $44556677",
        "($00007FDC).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveLogicalShiftLeftOffset) {
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
        "STR R1, [R2, R3, LSL #1]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeLogicalShiftLeftOffset) {
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
        "STR R1, [R2, -R3, LSL #1]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveLogicalShiftRightOffset) {
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
        "STR R1, [R2, R3, LSR #1]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeLogicalShiftRightOffset) {
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
        "STR R1, [R2, -R3, LSR #1]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveArithmeticShiftRightOffset) {
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
        "STR R1, [R2, R3, ASR #2]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeArithmeticShiftRightOffset) {
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
        "STR R1, [R2, -R3, ASR #2]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveRotateRightOffset) {
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
        "STR R1, [R2, R3, ROR #30]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeRotateRightOffset) {
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
        "STR R1, [R2, -R3, ROR #30]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, PositiveRotateRightExtendOffset) {
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
        "STR R1, [R2, R3, RRX]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00008020).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest, NegativeRotateRightExtendOffset) {
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
        "STR R1, [R2, -R3, RRX]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).W is $44556677",
        "($00007FE0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008010).W is $44556677"
    });
    When({
        "STR R15, [R2, R3]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008010).W is $C800100F"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPositionThenProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).W is $44556677",
        "($0000800C).W is $DDEE1122"
    });
    When({
        "STR R1, [R15, R2]!"
    });
    Then({
        "CYCLES is NNNS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).W is $AABBCCDD",
        "($0000800C).W is $DDEE1122"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).W is $44556677"
    });
    When({
        "STR R1, [R2, R15, LSL #6]!"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $000402C0",
        "($000402C0).W is $AABBCCDD"
    });
}

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
       AddressExceptionIsTakenWhenTheTransferAddressIsOutside26BitRangeAndTheBaseRegisterIsNotUpdated) {
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
        "STR R1, [R2, R3]!"
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

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
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
        "STR R1, [R2, R3]!"
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

TEST_F(StoreWordRegisterPreIndexedWritebackTest,
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
        "STR R1, [R2, R3]!"
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
