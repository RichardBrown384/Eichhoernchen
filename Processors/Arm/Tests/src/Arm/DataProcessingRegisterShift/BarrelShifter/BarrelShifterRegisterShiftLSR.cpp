#include "Arm/ArmTest.h"

class BarrelShifterRegisterShiftLSRTest : public ArmTest {};

TEST_F(BarrelShifterRegisterShiftLSRTest, ZeroShiftPropagatesCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000100"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
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

TEST_F(BarrelShifterRegisterShiftLSRTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000101"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000101"
    });
}

TEST_F(BarrelShifterRegisterShiftLSRTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000101"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000101"
    });
}

TEST_F(BarrelShifterRegisterShiftLSRTest, Shift32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftLSRTest, Shift32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $00000003",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftLSRTest, ShiftMoreThan32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000123"
    });
    When({
        "ORRS R2, R3, R4, LSR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000123"
    });
}