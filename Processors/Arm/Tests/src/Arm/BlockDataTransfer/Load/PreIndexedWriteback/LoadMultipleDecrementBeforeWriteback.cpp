#include "Arm/ArmTest.h"

class LoadMultipleDecrementBeforeWritebackTest: public ArmTest {};

TEST_F(LoadMultipleDecrementBeforeWritebackTest, BaseRegisterNotInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $33333333",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, BaseRegisterInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R7}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $00000000",
        "R7 is $33333333",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15IsInTheRegisterListWithPSRBitClear) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R15}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is 0,SVC",
        "PC is $03333338",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15IsInTheRegisterListWithPSRBitSetSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333330",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R15}^"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is CVIF,USR",
        "PC is $03333338",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333330",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15IsInTheRegisterListWithPSRBitSetUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333332",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R15}^"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is CV,USR",
        "PC is $03333338",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333332",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15NotInTheRegisterListWithPSRBitSetUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $0000800C",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,USR",
        "PC is $0000100C",
        "R1 is $1A111111",
        "R7 is $00008000",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R8_USR is $2B222222",
        "R9_USR is $3C333333",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15NotInTheRegisterListWithPSRBitClearFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $0000800C",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R8, R9}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $1A111111",
        "R7 is $00008000",
        "R8_FIQ is $2B222222",
        "R9_FIQ is $3C333333",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15NotInTheRegisterListWithPSRBitSetFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $0000800C",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $1A111111",
        "R7 is $00008000",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R8_USR is $2B222222",
        "R9_USR is $3C333333",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest,
       UserBankTransferWithWriteBackInFastInterruptModeUpdatesTheFastInterruptBaseRegister) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R10_FIQ is $0000800C",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "R10_USR is $BBBBBBBB",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R10!, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $1A111111",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R10_FIQ is $00008000",
        "R8_USR is $2B222222",
        "R9_USR is $3C333333",
        "R10_USR is $BBBBBBBB",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, R15IsUsedAsTheBaseRegister) {
    // The convention adopted here is that the PC
    // specified in the givens is 8 bytes ahead of the address
    // of the first instruction to be executed
    // What this means is that the instruction to be executed is at 0x100C
    // The PC is already advanced 8 bytes at 0x1014
    // The first address to be retrieved is 0x1014 - 3 * 4 = 0x1008
    // Therefore, the R3 should contain the value of the assembled instruction
    Given({
        "PSR is 0,SVC",
        "PC is $00001014",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00001008).W is $11111111",
        "($0000100C).W is $22222222",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
    When({
        "LDMDB R15!, {R1, R3, R7}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,SVC",
        "PC is $00001018",
        "R1 is $11111111",
        "R3 is $E93F008A",
        "R5 is $00000000",
        "R7 is $33333333",
        "($00001008).W is $11111111",
        "($0000100C).W is $E93F008A",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, NonWordAlignedBaseRegister) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800F",
        "($00008000).W is $00112233",
        "($00008004).W is $44556677",
        "($00008008).W is $8899AABB",
        "($0000800C).W is $CCDDEEFF"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00112233",
        "R3 is $44556677",
        "R5 is $8899AABB",
        "R7 is $00008003",
        "($00008000).W is $00112233",
        "($00008004).W is $44556677",
        "($00008008).W is $8899AABB",
        "($0000800C).W is $CCDDEEFF"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest,
       AddressExceptionIsTakenWhenBaseRegisterIsOutside26BitRange) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F000800C",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $DDDDDDDD",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008000",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $0000100B",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest,
       AddressExceptionIsTakenWhenR15WithPSRBitsIsOutside26BitRange) {
    Given({
        "PSR is C,USR",
        "PC is $00001014",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008008",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $DDDDDDDD",
        "($00001008).W is $11111111",
        "($0000100C).W is $22222222",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
    When({
        "LDMDB R15!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is CI,SVC",
        "PC is $0000001C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008008",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $20001014",
        "($00001008).W is $11111111",
        "($0000100C).W is $E93F002A",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest,
       TheSupervisorBaseRegisterIsUpdatedWhenAnAddressExceptionOccursDuringAUserBankTransferWithWriteBack) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R13_USR is $0000800C",
        "R13_SVC is $F000800C",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $DDDDDDDD",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMDB R13!, {R1, R3, R5}^"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R13_USR is $0000800C",
        "R13_SVC is $F0008000",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $0000100B",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, DataAbortOccursDuringSecondCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "R14_SVC is $0000FFFF",
        "($00008000).W is ABORT",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000100B",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, DataAbortOccursDuringThirdCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is ABORT",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, DataAbortOccursDuringFourthCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $0000800C",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $1A111111",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, DataAbortOccursDuringFourthCycleWithBaseRegisterFirstInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $0000800C",
        "R3 is $00000000",
        "R5 is $00000000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R1!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $00008000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest,
       WhenADataAbortOcurrsDuringAUserBankTransferWithWriteBackTheSupervisorBaseRegisterIsUpdated) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R13_USR is $00001000",
        "R13_SVC is $0000800C",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is ABORT",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R13!, {R1, R3, R5}^"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R13_USR is $00001000",
        "R13_SVC is $00008000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleDecrementBeforeWritebackTest, AnAddressExceptionTakesPrecidenceOverADataAbort) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $8000800C",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMDB R7!, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $80008000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($0000800C).W is $4D444444"
    });
}