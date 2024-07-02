#include "Arm/ArmTest.h"

class StoreMultipleIncrementBeforeTest: public ArmTest {};

TEST_F(StoreMultipleIncrementBeforeTest, BaseRegisterNotInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $1111111A",
        "($00008004).W is $2222222B",
        "($00008008).W is $3333333C",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, BaseRegisterInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R7}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $1111111A",
        "($00008004).W is $2222222B",
        "($00008008).W is $00007FFC",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, R15IsInTheRegisterListWithPSRBitClear) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R15}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $1111111A",
        "($00008004).W is $2222222B",
        "($00008008).W is $8000100F",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, R15IsInTheRegisterListWithPSRBitSetSupervisorMode) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R15}^"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $1111111A",
        "($00008004).W is $2222222B",
        "($00008008).W is $8000100F",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, R15IsInTheRegisterListWithPSRBitSetUserMode) {
    Given({
        "PSR is N,USR",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R15}^"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is N,USR",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $00007FFC",
        "($00008000).W is $1111111A",
        "($00008004).W is $2222222B",
        "($00008008).W is $8000100C",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, BankableRegistersInRegisterListPSRBitSetUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,USR",
        "PC is $0000100C",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $E111111E",
        "($00008004).W is $1AAAAAA2",
        "($00008008).W is $2BBBBBB3",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, BankableRegistersInRegisterListPSRBitClearFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R8, R9}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $E111111E",
        "($00008004).W is $B888888A",
        "($00008008).W is $C999999D",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, BankableRegistersInRegisterListPSRBitSetFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $E111111E",
        "R7 is $00007FFC",
        "R8_FIQ is $B888888A",
        "R9_FIQ is $C999999D",
        "R8_USR is $1AAAAAA2",
        "R9_USR is $2BBBBBB3",
        "($00008000).W is $E111111E",
        "($00008004).W is $1AAAAAA2",
        "($00008008).W is $2BBBBBB3",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, BankableRegistersInRegisterListPSRBitSetFastInterruptModeBaseBanked) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R1 is $E111111E",
        "R8_FIQ is $F222222F",
        "R9_FIQ is $A333333A",
        "R10_FIQ is $00007FFC",
        "R8_USR is $1A1A1A1A",
        "R9_USR is $2B2B2B2B",
        "R10_USR is $3CBCBCBC",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R10, {R1, R8, R9}^"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,FIQ",
        "PC is $0000100C",
        "R1 is $E111111E",
        "R8_FIQ is $F222222F",
        "R9_FIQ is $A333333A",
        "R10_FIQ is $00007FFC",
        "R8_USR is $1A1A1A1A",
        "R9_USR is $2B2B2B2B",
        "R10_USR is $3CBCBCBC",
        "($00008000).W is $E111111E",
        "($00008004).W is $1A1A1A1A",
        "($00008008).W is $2B2B2B2B",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, R15IsUsedAsTheBaseRegister) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $4444444D",
        "($0000100C).W is $00000000",
        "($00001010).W is $00000000",
        "($00001014).W is $00000000",
        "($00001018).W is $00000000"
    });
    When({
        "STMIB R15, {R1, R3, R7}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $1111111A",
        "R3 is $2222222B",
        "R5 is $3333333C",
        "R7 is $4444444D",
        "($0000100C).W is $1111111A",
        "($00001010).W is $2222222B",
        "($00001014).W is $4444444D",
        "($00001018).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, NonWordAlignedBaseRegister) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFF",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFF",
        "($00008000).W is $B111111A",
        "($00008004).W is $B222222B",
        "($00008008).W is $B333333C",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest,
       AddressExceptionIsTakenWhenBaseRegisterIsOutside26BitRange) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $F0007FFC",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $DDDDDDDD",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $F0007FFC",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $0000100B",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest,
       AddressExceptionIsTakenWhenR15WithPSRBitsIsOutside26BitRange) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $F0008000",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $DDDDDDDD",
        "($0000100C).W is $00000000",
        "($00001010).W is $00000000",
        "($00001014).W is $00000000",
        "($00001018).W is $00000000"
    });
    When({
        "STMIB R15, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is CI,SVC",
        "PC is $0000001C",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $F0008000",
        "R14_USR is $AAAAAAAA",
        "R14_FIQ is $BBBBBBBB",
        "R14_IRQ is $CCCCCCCC",
        "R14_SVC is $20001008",
        "($0000100C).W is $00000000",
        "($00001010).W is $00000000",
        "($00001014).W is $00000000",
        "($00001018).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, DataAbortOccursDuringSecondCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000FFFF",
        "($00008000).W is ABORT",
        "($00008004).W is $00000000",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000100B",
        "($00008004).W is $B222222B",
        "($00008008).W is $B333333C",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, DataAbortOccursDuringThirdCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $00000000",
        "($00008004).W is ABORT",
        "($00008008).W is $00000000",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000100B",
        "($00008000).W is $B111111A",
        "($00008008).W is $B333333C",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, DataAbortOccursDuringFourthCycle) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is ABORT",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $00007FFC",
        "R14_SVC is $0000100B",
        "($00008000).W is $B111111A",
        "($00008004).W is $B222222B",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, DataAbortOccursDuringFourthCycleWithBaseRegisterFirstInList) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00007FFC",
        "R3 is $BBCCDDEE",
        "R5 is $AAFF9988",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is ABORT",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R1, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $00000018",
        "R1 is $00007FFC",
        "R3 is $BBCCDDEE",
        "R5 is $AAFF9988",
        "R14_SVC is $0000100B",
        "($00008000).W is $00007FFC",
        "($00008004).W is $BBCCDDEE",
        "($0000800C).W is $00000000"
    });
}

TEST_F(StoreMultipleIncrementBeforeTest, AnAddressExceptionTakesPrecidenceOverADataAbort) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $80007FFC",
        "R14_SVC is $0000FFFF",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($00008008).W is ABORT",
        "($0000800C).W is $00000000"
    });
    When({
        "STMIB R7, {R1, R3, R5}"
    });
    Then({
        "CYCLES is NNSSNS",
        "PSR is I,SVC",
        "PC is $0000001C",
        "R1 is $B111111A",
        "R3 is $B222222B",
        "R5 is $B333333C",
        "R7 is $80007FFC",
        "R14_SVC is $0000100B",
        "($00008000).W is $00000000",
        "($00008004).W is $00000000",
        "($0000800C).W is $00000000"
    });
}