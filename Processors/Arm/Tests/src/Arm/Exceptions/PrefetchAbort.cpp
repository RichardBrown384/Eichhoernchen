#include "Arm/ArmTest.h"

class PrefetchAbortTest: public ArmTest {};

TEST_F(PrefetchAbortTest, PrefetchAbortIsTakenWhenAPrefetchAbortIsPresented) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000000C).ASM is MOV R0, R0", // No-op
        "($00000010).ASM is MOV R0, R0",
        "($00001008).W is ABORT"
    });
    When({
        "MOV R0, R0",
        "MOV R0, R0",
    }, 1u); // Run with one additional invocation to trigger the prefetch abort
    Then({
        "CYCLES is SSNS",
        "PSR is I,SVC",
        "PC is $00000014", // Processor is already 8 bytes ahead of the instruction about to be executed
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($0000000C).ASM is MOV R0, R0",
        "($00000010).ASM is MOV R0, R0",
    });
}

TEST_F(PrefetchAbortTest, TrivialExceptionHandlerReturnsToInstruction) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($0000000C).ASM is SUBS R15, R14, #4",
        "($00000010).ASM is MOV R0, R0",
        "($00001008).W is ABORT"
    });
    When({
         "MOV R0, R0",
         "MOV R0, R0",
    }, 2u); // Run prefetch abort + trivial exception handler
    Then({
        "CYCLES is SSNSSNS",
        "PSR is 0,SVC",
        "PC is $000001010", // Program counter is 8 ahead of the original abort address
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($0000000C).ASM is SUBS R15, R14, #4",
        "($00000010).ASM is MOV R0, R0",
    });
}

