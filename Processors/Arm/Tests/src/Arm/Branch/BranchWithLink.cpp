#include "Arm/ArmTest.h"

class BranchWithLinkTest: public ArmTest {};

TEST_F(BranchWithLinkTest, BranchTakenSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BL #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,SVC",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $20001007"
    });
}

TEST_F(BranchWithLinkTest, BranchNotTakenSupervisorMode) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BLCC #504"
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

TEST_F(BranchWithLinkTest, BranchTakenInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BL #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,IRQ",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $20001006",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchWithLinkTest, BranchNotTakenInterruptMode) {
    Given({
        "PSR is C,IRQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BLCC #504"
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

TEST_F(BranchWithLinkTest, BranchTakenFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BL #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,FIQ",
        "PC is $00001200",
        "R14_USR is $11111111",
        "R14_FIQ is $20001005",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchWithLinkTest, BranchNotTakenFastInterruptMode) {
    Given({
        "PSR is C,FIQ",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BLCC #504"
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

TEST_F(BranchWithLinkTest, BranchTakenUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BL #504"
    });
    Then({
        "CYCLES is SNS",
        "PSR is C,USR",
        "PC is $00001200",
        "R14_USR is $20001004",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
}

TEST_F(BranchWithLinkTest, BranchNotTakenUserMode) {
    Given({
        "PSR is C,USR",
        "PC is $00001008",
        "R14_USR is $11111111",
        "R14_FIQ is $22222222",
        "R14_IRQ is $33333333",
        "R14_SVC is $44444444"
    });
    When({
        "BLCC #504"
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