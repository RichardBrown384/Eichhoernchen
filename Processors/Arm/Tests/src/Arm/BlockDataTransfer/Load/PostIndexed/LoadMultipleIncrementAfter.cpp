#include "Arm/ArmTest.h"

class LoadMultipleIncrementAfterTest: public ArmTest {};

TEST_F(LoadMultipleIncrementAfterTest, BaseRegisterNotInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest, BaseRegisterInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMIA R7, {R1, R3, R7}"
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

TEST_F(LoadMultipleIncrementAfterTest, R15IsInTheRegisterListWithPSRBitClear) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $33333333",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMIA R7, {R1, R3, R15}"
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

TEST_F(LoadMultipleIncrementAfterTest, R15IsInTheRegisterListWithPSRBitSetSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333330",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMIA R7, {R1, R3, R15}^"
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

TEST_F(LoadMultipleIncrementAfterTest, R15IsInTheRegisterListWithPSRBitSetUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "($00008000).W is $11111111",
        "($00008004).W is $22222222",
        "($00008008).W is $3F333332",
        "($0000800C).W is $44444444"
    });
    When({
        "LDMIA R7, {R1, R3, R15}^"
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

TEST_F(LoadMultipleIncrementAfterTest, R15NotInTheRegisterListWithPSRBitSetUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $00008000",
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
        "LDMIA R7, {R1, R8, R9}^"
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

TEST_F(LoadMultipleIncrementAfterTest, R15NotInTheRegisterListWithPSRBitClearFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $00008000",
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
        "LDMIA R7, {R1, R8, R9}"
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

TEST_F(LoadMultipleIncrementAfterTest, R15NotInTheRegisterListWithPSRBitSetFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R7 is $00008000",
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
        "LDMIA R7, {R1, R8, R9}^"
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

TEST_F(LoadMultipleIncrementAfterTest, R15NotInTheRegisterListWithPSRBitSetFastInterruptModeBaseBanked) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $00000000",
        "R8_FIQ is $00000000",
        "R9_FIQ is $00000000",
        "R10_FIQ is $00008000",
        "R8_USR is $AAAAAAAA",
        "R9_USR is $AAAAAAAA",
        "R10_USR is $BBBBBBBB",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R10, {R1, R8, R9}^"
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

TEST_F(LoadMultipleIncrementAfterTest, R15IsUsedAsTheBaseRegister) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
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
        "LDMIA R15, {R1, R3, R7}"
    });
    Then({
        "CYCLES is SNSSI",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $11111111",
        "R3 is $22222222",
        "R5 is $00000000",
        "R7 is $33333333",
        "($00001008).W is $11111111",
        "($0000100C).W is $22222222",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
}

TEST_F(LoadMultipleIncrementAfterTest, NonWordAlignedBaseRegister) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008003",
        "($00008000).W is $00112233",
        "($00008004).W is $44556677",
        "($00008008).W is $8899AABB",
        "($0000800C).W is $CCDDEEFF"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest,
       AddressExceptionIsTakenWhenBaseRegisterIsOutside26BitRange) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008000",
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
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest,
       AddressExceptionIsTakenWhenR15WithPSRBitsIsOutside26BitRange) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008000",
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
        "LDMIA R15, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is CI,SVC",
        "PC is $0000001C",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $F0008000",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $20001008",
        "($00001008).W is $11111111",
        "($0000100C).W is $22222222",
        "($00001010).W is $33333333",
        "($00001014).W is $44444444"
    });
}

TEST_F(LoadMultipleIncrementAfterTest, DataAbortOccursDuringSecondCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is ABORT",
        "($00008004).W is $2B222222",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest, DataAbortOccursDuringThirdCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is ABORT",
        "($00008008).W is $3C333333",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest, DataAbortOccursDuringFourthCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $00008000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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

TEST_F(LoadMultipleIncrementAfterTest, DataAbortOccursDuringFourthCycleWithBaseRegisterFirstInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00008000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R1, {R1, R3, R5}"
    });
    Then({
        "CYCLES is SNSSINS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $1A111111",
        "R3 is $00000000",
        "R5 is $00000000",
        "R14_SVC is $0000100B",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($0000800C).W is $4D444444"
    });
}

TEST_F(LoadMultipleIncrementAfterTest, AnAddressExceptionTakesPrecidenceOverADataAbort) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R3 is $00000000",
        "R5 is $00000000",
        "R7 is $80008000",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $1A111111",
        "($00008004).W is $2B222222",
        "($00008008).W is ABORT",
        "($0000800C).W is $4D444444"
    });
    When({
        "LDMIA R7, {R1, R3, R5}"
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