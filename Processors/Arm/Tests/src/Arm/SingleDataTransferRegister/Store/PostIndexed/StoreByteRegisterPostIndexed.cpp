#include "Arm/ArmTest.h"

class StoreByteRegisterPostIndexedTest: public ArmTest {};

TEST_F(StoreByteRegisterPostIndexedTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $77"
    });
    When({
        "STRB R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $EE"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveOffset) {
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
        "STRB R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NonAlignedPositiveOffset) {
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
        "STRB R1, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008021",
        "R3 is $00000020",
        "($00008001).B is $EE",
        "($00008021).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeOffset) {
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
        "STRB R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NonAlignedNegativeOffset) {
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
        "STRB R1, [R2], -R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE1",
        "R3 is $00000020",
        "($00008001).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveLogicalShiftLeftOffset) {
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
        "STRB R1, [R2], R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeLogicalShiftLeftOffset) {
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
        "STRB R1, [R2], -R3, LSL #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveLogicalShiftRightOffset) {
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
        "STRB R1, [R2], R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeLogicalShiftRightOffset) {
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
        "STRB R1, [R2], -R3, LSR #1"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveArithmeticShiftRightOffset) {
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
        "STRB R1, [R2], R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeArithmeticShiftRightOffset) {
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
        "STRB R1, [R2], -R3, ASR #2"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveRotateRightOffset) {
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
        "STRB R1, [R2], R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeRotateRightOffset) {
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
        "STRB R1, [R2], -R3, ROR #30"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, PositiveRotateRightExtendOffset) {
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
        "STRB R1, [R2], R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00008020).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest, NegativeRotateRightExtendOffset) {
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
        "STRB R1, [R2], -R3, RRX"
    });
    Then({
        "CYCLES is NN",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $118899EE",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $EE",
        "($00007FE0).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest,
       ProgramCounterIsPlus12AndPSRBitsArePresentedWhenR15IsInRdPosition) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $77"
    });
    When({
        "STRB R15, [R2], R3"
    });
    Then({
        "CYCLES is NN",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008000).B is $0F"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest,
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
        "STRB R1, [R15], R2"
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

TEST_F(StoreByteRegisterPostIndexedTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00004000",
        "($00004000).B is $77"
    });
    When({
        "STRB R1, [R2], R15, LSL #6"
    });
    Then({
        "CYCLES is NN",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $AABBCCDD",
        "R2 is $000442C0",
        "($00004000).B is $DD"
    });
}

TEST_F(StoreByteRegisterPostIndexedTest,
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
        "STRB R1, [R2], R3"
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

TEST_F(StoreByteRegisterPostIndexedTest,
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
        "STRB R1, [R2], R3"
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

TEST_F(StoreByteRegisterPostIndexedTest,
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
        "STRB R1, [R2], R3"
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