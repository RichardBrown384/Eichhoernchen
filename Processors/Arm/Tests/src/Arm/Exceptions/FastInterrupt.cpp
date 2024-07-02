#include "Arm/ArmTest.h"

// Note: these tests are dependent on an implementation detail
// When an FIRQ is serviced, the Arm::Execute method immediately returns

class FastInterruptTest: public ArmTest {};

TEST_F(FastInterruptTest, FastInterruptIsNotTakenWhenTheFirqLineItNotAsserted) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is 0",
        "($0000001C).ASM is MOV R0, R0", // No-op
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R1 is $00000002",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
}

TEST_F(FastInterruptTest, FastInterruptIsNotTakenWhenTheFirqLineIsAssertedAndTheFBitIsSet) {
    Given({
        "PSR is CF,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is F",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is S",
        "PSR is CF,SVC",
        "PC is $0000100C",
        "R1 is $00000002",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
}

TEST_F(FastInterruptTest, FastInterruptIsTakenWhenTheFirqLineIsAssertedAndTheFBitIsClear) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is F",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is NS",
        "PSR is CIF,FIQ",
        "PC is $00000024",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $20001007",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
}

TEST_F(FastInterruptTest, FastInterruptIsTakenWhenTheFirqLineIsAssertedAndTheIBitIsSet) {
    Given({
        "PSR is CI,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is F",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is NS",
        "PSR is CIF,FIQ",
        "PC is $00000024",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $28001007",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
}

TEST_F(FastInterruptTest, FastInterruptIsTakenWhenBothIrqAndFirqLineIsAssertedAndTheIAndFBitsAreClear) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is IF",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is NS",
        "PSR is CIF,FIQ",
        "PC is $00000024",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $20001007",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is MOV R0, R0",
        "($00000020).ASM is MOV R0, R0",
    });
}

TEST_F(FastInterruptTest, TrivialExceptionHandlerReturnsToInstruction) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is F",
        "($0000001C).ASM is SUBS R15, R14, #4",
        "($00000020).ASM is MOV R0, R0",
    });
    When({
        "ADD R1, R1, R1",
    }, 1u); // Run the exception handler
    Then({
        "CYCLES is NSSNS",
        "PSR is C,SVC",
        "PC is $00001008", // The cpu has already returned from the handler and is ready to resume execution
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $20001007",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000001C).ASM is SUBS R15, R14, #4",
        "($00000020).ASM is MOV R0, R0",
    });
}
