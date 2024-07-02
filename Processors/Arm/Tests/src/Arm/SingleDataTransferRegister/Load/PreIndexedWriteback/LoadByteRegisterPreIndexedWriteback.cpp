#include "Arm/ArmTest.h"

class LoadByteRegisterPreIndexedWritebackTest: public ArmTest {};

TEST_F(LoadByteRegisterPreIndexedWritebackTest, ZeroOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $44"
    });
    When({
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000044",
        "R2 is $00008000",
        "R3 is $00000000",
        "($00008000).B is $44"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NonAlignedPositiveOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00008020).B is $DD",
        "($00008021).B is $EE"
    });
    When({
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000EE",
        "R2 is $00008021",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00008020).B is $DD",
        "($00008021).B is $EE"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000020",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NonAlignedNegativeOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00007FE0).B is $DD",
        "($00007FDF).B is $EE"
    });
    When({
        "LDRB R1, [R2, -R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000EE",
        "R2 is $00007FDF",
        "R3 is $00000021",
        "($00008000).B is $44",
        "($00007FE0).B is $DD",
        "($00007FDF).B is $EE"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3, LSL #1]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeLogicalShiftLeftOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3, LSL #1]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000010",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3, LSR #1]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeLogicalShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3, LSR #1]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3, ASR #2]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeArithmeticShiftRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3, ASR #2]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000080",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3, ROR #30]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeRotateRightOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3, ROR #30]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000008",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, PositiveRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
    When({
        "LDRB R1, [R2, R3, RRX]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00008020",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00008020).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest, NegativeRotateRightExtendOffset) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00008000",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
    When({
        "LDRB R1, [R2, -R3, RRX]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $000000DD",
        "R2 is $00007FE0",
        "R3 is $00000040",
        "($00008000).B is $44",
        "($00007FE0).B is $DD"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
       WhenTheSameRegisterOccupiesTheRdAndRnPositionsThenRdWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008010).B is $77"
    });
    When({
        "LDRB R2, [R2, R3]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is NZI,SVC",
        "PC is $0000100C",
        "R2 is $00000077",
        "R3 is $00000010",
        "($00008010).B is $77"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
       WhenR15IsInRdPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R2 is $00008000",
        "R3 is $00000010",
        "($00008010).B is $77"
    });
    When({
        "LDRB R15, [R2, R3]!"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "R2 is $00008010",
        "R3 is $00000010",
        "($00008010).B is $77"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
       WhenR15IsInRnPositionTheProgramCounterIsUpdated) {
    Given({
        "PSR is ZIF,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00007000",
        "($00008008).B is $77",
        "($0000800C).B is $22"
    });
    When({
        "LDRB R1, [R15, R2]!"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is ZIF,SVC",
        "PC is $00008010",
        "R1 is $00000077",
        "R2 is $00007000",
        "($00008008).B is $77",
        "($0000800C).B is $22"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
       ProgramCounterIsPlus8AndPSRBitsArePresentedWhenR15IsInRmPosition) {
    Given({
        "PSR is F,SVC",
        "PC is $00001008",
        "R1 is $AABBCCDD",
        "R2 is $00000000",
        "($000402C0).B is $77"
    });
    When({
        "LDRB R1, [R2, R15, LSL #6]!"
    });
    Then({
        "CYCLES is SNI",
        "PSR is F,SVC",
        "PC is $0000100C",
        "R1 is $00000077",
        "R2 is $000402C0",
        "($000402C0).B is $77"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
       WhenTheProgramCounterOccupiesTheRdAndRnPositionsThenProgramCounterWillContainTheValueLoadedFromMemory) {
    Given({
        "PSR is NZI,SVC",
        "PC is $00001008",
        "R3 is $00007000",
        "($00008008).B is $77"
    });
    When({
        "LDRB R15, [R15, R3]!"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is NZI,SVC",
        "PC is $0000007C",
        "R3 is $00007000",
        "($00008008).B is $77"
    });
}

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
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
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNINS",
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

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
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
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNINS",
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

TEST_F(LoadByteRegisterPreIndexedWritebackTest,
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
        "LDRB R1, [R2, R3]!"
    });
    Then({
        "CYCLES is SNINS",
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