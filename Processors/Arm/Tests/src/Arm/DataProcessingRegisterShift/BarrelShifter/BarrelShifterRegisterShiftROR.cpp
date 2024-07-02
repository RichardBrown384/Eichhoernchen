#include "Arm/ArmTest.h"

class BarrelShifterRegisterShiftRORTest : public ArmTest {};

TEST_F(BarrelShifterRegisterShiftRORTest, ZeroShiftPropagatesCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000100"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000100"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000102"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000001",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000102"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000102"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $40000001",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000102"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000003",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000003",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift35SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000007",
        "R5 is $00000123"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $E0000001",
        "R3 is $00000001",
        "R4 is $00000007",
        "R5 is $00000123"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift35ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000123"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $60000001",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000123"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift64SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000203",
        "R5 is $00000140"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000203",
        "R3 is $00000001",
        "R4 is $80000203",
        "R5 is $00000140"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift64ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000203",
        "R5 is $00000140"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000203",
        "R3 is $00000001",
        "R4 is $00000203",
        "R5 is $00000140"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift234SetsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000203",
        "R5 is $000001EA"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80C00001",
        "R3 is $00000001",
        "R4 is $00000203",
        "R5 is $000001EA"
    });
}

TEST_F(BarrelShifterRegisterShiftRORTest, Shift234ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $000001EA"
    });
    When({
        "ORRS R2, R3, R4, ROR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00C00001",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $000001EA"
    });
}

