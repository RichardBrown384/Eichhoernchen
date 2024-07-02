#include "Arm/ArmTest.h"

class UndefinedInstructionTest : public ArmTest {};

TEST_F(UndefinedInstructionTest, UndefinedInstructionExceptionIsTakenWhenAnUndefinedInstructionIsPresented) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($00000004).ASM is MOV R0, R0",
        "($00000008).ASM is MOV R0, R0"
    });
    When({
        0xE7FF'FFFFu, // Undefined LDR
    });
    Then({
        "CYCLES is SINS",
        "PSR is I,SVC",
        "PC is $0000000C", // Processor is already 8 bytes ahead of the instruction about to be executed
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001004",
        "($00000004).ASM is MOV R0, R0",
        "($00000008).ASM is MOV R0, R0"
    });
}

TEST_F(UndefinedInstructionTest, TrivialExceptionHandlerReturnsToNextInstruction) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($00000004).ASM is MOVS R15, R14",
        "($00000008).ASM is MOV R0, R0"
    });
    When({
        0xE7FF'FFFFu, // Undefined LDR
    }, 1u); // Run the exception handler
    Then({
        "CYCLES is SINSSNS",
        "PSR is 0,USR",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001004",
        "($00000004).ASM is MOVS R15, R14",
        "($00000008).ASM is MOV R0, R0"
    });
}