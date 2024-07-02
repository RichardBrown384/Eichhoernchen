#include "Arm/ArmTest.h"

class CoprocessorDataOperationTest: public ArmTest {};

TEST_F(CoprocessorDataOperationTest,
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
        "CDP p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDPCC p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDP p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDPCC p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDP p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDPCC p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDP p7, #7, c1, c3, c4"
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

TEST_F(CoprocessorDataOperationTest,
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
        "CDPCC p7, #7, c1, c3, c4"
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