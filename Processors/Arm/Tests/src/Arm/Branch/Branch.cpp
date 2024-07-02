#include "Arm/ArmTest.h"

class BranchTest: public ArmTest {};

TEST_F(BranchTest, BranchTakenSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "B #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,SVC",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchTest, BranchNotTakenSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BCC #504"
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

TEST_F(BranchTest, BranchTakenInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "B #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,IRQ",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchTest, BranchNotTakenInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BCC #504"
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

TEST_F(BranchTest, BranchTakenFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "B #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,FIQ",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchTest, BranchNotTakenFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BCC #504"
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

TEST_F(BranchTest, BranchTakenUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "B #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,USR",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchTest, BranchNotTakenUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BCC #504"
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