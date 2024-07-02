#include "Arm/ArmTest.h"

class SoftwareInterruptTest: public ArmTest {};

TEST_F(SoftwareInterruptTest, SoftwareInterruptSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $20001007"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptSetSupervisorModeWithISet) {
    Given({
        "PSR is CI,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $28001007"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptSupervisorModeNotTaken) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWICC 600"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $20001006"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptInterruptModeWithISet) {
    Given({
        "PSR is CI,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $28001006"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptInterruptModeNotTaken) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWICC 600"
    });
    Then({
        "CYCLES is S",
        "PSR is C,IRQ",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $20001005"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptFastInterruptModeWithISet) {
    Given({
        "PSR is CI,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $28001005"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptFastInterruptModeNotTaken) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWICC 600"
    });
    Then({
        "CYCLES is S",
        "PSR is C,FIQ",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $20001004"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptUserModeWithISet) {
    Given({
        "PSR is CI,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWI 600"
    });
    Then({
        "CYCLES is SNS",
        "PSR is CI,SVC",
        "PC is $00000010",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $28001004"
    });
}

TEST_F(SoftwareInterruptTest, SoftwareInterruptUserModeNotTaken) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "SWICC 600"
    });
    Then({
        "CYCLES is S",
        "PSR is C,USR",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(SoftwareInterruptTest, TrivialExceptionHandlerReturnsToNextInstruction) {
    Given({
        "PSR is 0,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444",
        "($00000008).ASM is MOVS R15, R14"
    });
    When({
        "SWI 600"
    }, 1u);
    Then({
        "CYCLES is SNSSNS",
        "PSR is 0,USR",
        "PC is $0000100C",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $00001004",
        "($00000008).ASM is MOVS R15, R14"
    });
}