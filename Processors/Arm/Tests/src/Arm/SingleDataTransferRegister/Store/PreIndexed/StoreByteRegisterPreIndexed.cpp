#include "Arm/ArmTest.h"

class StoreByteRegisterPreIndexedTest: public ArmTest {};

TEST_F(StoreByteRegisterPreIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $44"
    });
    When({
        "STRB R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00008021).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00007FDF).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00007FDF).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3, LSL #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3, LSL #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3, LSR #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3, LSR #1]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3, ASR #2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3, ASR #2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3, ROR #30]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3, ROR #30]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $EE"
    });
    When({
        "STRB R1, [R2, R3, RRX]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $EE"
    });
    When({
        "STRB R1, [R2, -R3, RRX]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $77"
    });
    When({
        "STRB R15, [R2, R3]"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $0F"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsAreNotPresentedWhenR15IsInRnPosition) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).B is $77",
        "($0000800C).B is $22"
    });
    When({
        "STRB R1, [R15, R2]"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).B is $DD",
        "($0000800C).B is $22"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).B is $77"
    });
    When({
        "STRB R1, [R2, R15, LSL #6]"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPreIndexedTest,
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
        "STRB R1, [R2, R3]"
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

TEST_F(StoreByteRegisterPreIndexedTest,
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
        "STRB R1, [R2, R3]"
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

TEST_F(StoreByteRegisterPreIndexedTest,
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
        "STRB R1, [R2, R3]"
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