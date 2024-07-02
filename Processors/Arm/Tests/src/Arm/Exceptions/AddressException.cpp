#include "Arm/ArmTest.h"

class AddressExceptionTest : public ArmTest {};

TEST_F(AddressExceptionTest, AddressExceptionIsTakenWhenAnInvalidAddressIsPresented) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0002000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($00000014).ASM is MOV R0, R0",
        "($00000018).ASM is MOV R0, R0",
        "($00002000).W is $33BBBB33"
    });
    When({
        "LDR R2, [R3]"
    });
    Then({
        "CYCLES is SNINS",
        "PSR is I,SVC",
        "PC is $0000001C", // Processor is already 8 bytes ahead of the instruction about to be executed
        "R2 is $00000000",
        "R3 is $F0002000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001008",
        "($00000014).ASM is MOV R0, R0",
        "($00000018).ASM is MOV R0, R0"
    });
}

TEST_F(AddressExceptionTest, TrivialExceptionHandlerReturnsToNextInstruction) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $F0002000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $0000100F",
        "($00000014).ASM is SUBS R15, R14, #4",
        "($00000018).ASM is MOV R0, R0",
        "($00002000).W is $33BBBB33"
    });
    When({
        "LDR R2, [R3]"
    }, 1u); // Run exception handler
    Then({
        "CYCLES is SNINSSNS",
        "PSR is 0,USR",
        "PC is $0000100C", // Processor is already 8 bytes ahead of the instruction about to be executed
        "R2 is $00000000",
        "R3 is $F0002000",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001008",
        "($00000014).ASM is SUBS R15, R14, #4",
        "($00000018).ASM is MOV R0, R0",
        "($00002000).W is $33BBBB33"
    });
}