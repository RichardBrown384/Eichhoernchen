#include "Arm/ArmTest.h"

class BarrelShifterInstructionShiftLSRTest : public ArmTest {};

TEST_F(BarrelShifterInstructionShiftLSRTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000003"
    });
    When({
        "ORRS R2, R3, R4, LSR #1"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000003"
    });
}

TEST_F(BarrelShifterInstructionShiftLSRTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000002"
    });
    When({
        "ORRS R2, R3, R4, LSR #1"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000002"
    });
}

TEST_F(BarrelShifterInstructionShiftLSRTest, Shift32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $FFFFFFFF"
    });
    When({
        "ORRS R2, R3, R4, LSR #32"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00001000",
        "R3 is $00001000",
        "R4 is $FFFFFFFF"
    });
}

TEST_F(BarrelShifterInstructionShiftLSRTest, Shift32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00001000",
        "R4 is $7FFFFFFF"
    });
    When({
        "ORRS R2, R3, R4, LSR #32"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001000",
        "R3 is $00001000",
        "R4 is $7FFFFFFF"
    });
}