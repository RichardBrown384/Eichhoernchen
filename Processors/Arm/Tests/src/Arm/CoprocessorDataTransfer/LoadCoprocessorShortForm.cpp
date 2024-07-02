#include "Arm/ArmTest.h"

class LoadCoprocessorShortFormTest: public ArmTest {};

TEST_F(LoadCoprocessorShortFormTest,
       UndefinedInstructionExceptionTakenWhenCoprocessorAbsentSupervisorMode) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDC p7, c3, [R5]"
    });
    Then({
        "CYCLES is SINS",
        "PSR is I,SVC",
        "PC is $0000000C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001007"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UnexecutedInstructionShouldNotTakeExceptionWhenCoprocessorAbsentSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDCCC p7, c3, [R5]"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UndefinedInstructionExceptionTakenWhenCoprocessorAbsentInterruptMode) {
    Given({
        "PSR is 0,IRQ",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDC p7, c3, [R5]"
    });
    Then({
        "CYCLES is SINS",
        "PSR is I,SVC",
        "PC is $0000000C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001006"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UnexecutedInstructionShouldNotTakeExceptionWhenCoprocessorAbsentInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDCCC p7, c3, [R5]"
    });
    Then({
        "CYCLES is S",
        "PSR is C,IRQ",
        "PC is $0000100C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UndefinedInstructionExceptionTakenWhenCoprocessorAbsentFastInterruptMode) {
    Given({
        "PSR is 0,FIQ",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDC p7, c3, [R5]"
    });
    Then({
        "CYCLES is SINS",
        "PSR is I,SVC",
        "PC is $0000000C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001005"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UnexecutedInstructionShouldNotTakeExceptionWhenCoprocessorAbsentFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDCCC p7, c3, [R5]"
    });
    Then({
        "CYCLES is S",
        "PSR is C,FIQ",
        "PC is $0000100C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UndefinedInstructionExceptionTakenWhenCoprocessorAbsentUserMode) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDC p7, c3, [R5]"
    });
    Then({
        "CYCLES is SINS",
        "PSR is I,SVC",
        "PC is $0000000C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001004"
    });
}

TEST_F(LoadCoprocessorShortFormTest,
       UnexecutedInstructionShouldNotTakeExceptionWhenCoprocessorAbsentUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    }),
    When({
        "LDCCC p7, c3, [R5]"
    });
    Then({
        "CYCLES is S",
        "PSR is C,USR",
        "PC is $0000100C",
        "R5 is $00000004",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}