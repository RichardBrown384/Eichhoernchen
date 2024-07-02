#include "Arm/ArmTest.h"

// Note: these tests are dependent on an implementation detail
// When an IRQ is serviced, the Arm::Execute method returns immediately

class InterruptTest: public ArmTest {};

TEST_F(InterruptTest, InterruptIsNotTakenWhenTheIrqLineItNotAsserted) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is 0",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
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
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
}

TEST_F(InterruptTest, InterruptIsNotTakenWhenTheIrqLineIsAssertedAndTheIBitIsSet) {
    Given({
        "PSR is CI,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is I",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is S",
        "PSR is CI,SVC",
        "PC is $0000100C",
        "R1 is $00000002",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
}

TEST_F(InterruptTest, InterruptIsTakenWhenTheIrqLineIsAssertedAndTheIBitIsClear) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is I",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is NS",
        "PSR is CI,IRQ",
        "PC is $00000020",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $20001007",
        "R14_SVC is $44444444",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
}

TEST_F(InterruptTest, InterruptIsTakenWhenTheIrqLineIsAssertedAndTheFBitIsSet) {
    Given({
        "PSR is CF,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is I",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
    When({
        "ADD R1, R1, R1"
    });
    Then({
        "CYCLES is NS",
        "PSR is CIF,IRQ",
        "PC is $00000020",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $24001007",
        "R14_SVC is $44444444",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
}

TEST_F(InterruptTest, FastInterruptIsTakenWhenBothIrqAndIrqLineIsAssertedAndTheIAndFBitsAreClear) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is IF",
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
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
        "($00000018).ASM is MOV R0, R0",
        "($0000001C).ASM is MOV R0, R0"
    });
}

TEST_F(InterruptTest, TrivialExceptionHandlerReturnsToInstruction) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000001",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "IRQ is I",
        "($00000018).ASM is SUBS R15, R14, #4",
        "($0000001C).ASM is MOV R0, R0",
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
        "R14_FIQ is $22222222",
        "R14_IRQ is $20001007",
        "R14_SVC is $44444444",
        "($00000018).ASM is SUBS R15, R14, #4",
        "($0000001C).ASM is MOV R0, R0",
    });
}
