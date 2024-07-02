#include "Arm/ArmTest.h"

class ConditionCodeCSTest: public ArmTest {};

TEST_F(ConditionCodeCSTest, StatusRegister0) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterV) {
    Given({
        "PSR is V,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is V,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterC) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterCV) {
    Given({
        "PSR is CV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is CV,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterZ) {
    Given({
        "PSR is Z,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is Z,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterZV) {
    Given({
        "PSR is ZV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is ZV,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterZC) {
    Given({
        "PSR is ZC,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is ZC,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterZCV) {
    Given({
        "PSR is ZCV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is ZCV,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterN) {
    Given({
        "PSR is N,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNV) {
    Given({
        "PSR is NV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NV,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNC) {
    Given({
        "PSR is NC,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNCV) {
    Given({
        "PSR is NCV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NCV,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNZ) {
    Given({
        "PSR is NZ,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NZ,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNZV) {
    Given({
        "PSR is NZV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NZV,SVC",
        "PC is $0000100C",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNZC) {
    Given({
        "PSR is NZC,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NZC,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}

TEST_F(ConditionCodeCSTest, StatusRegisterNZCV) {
    Given({
        "PSR is NZCV,SVC",
        "PC is $00001008",
        "R1 is $00000000",
        "R2 is $00000003",
        "R3 is $00000003"
    });
    When({
        "ADDCS R1, R2, R3"
    });
    Then({
        "CYCLES is S",
        "PSR is NZCV,SVC",
        "PC is $0000100C",
        "R1 is $00000006",
        "R2 is $00000003",
        "R3 is $00000003"
    });
}