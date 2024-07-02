#include "Arm/ArmTest.h"

class BarrelShifterInstructionShiftLSLTest : public ArmTest {};

TEST_F(BarrelShifterInstructionShiftLSLTest, ZeroShiftPropagatesCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "ORRS R2, R3, R4"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}

TEST_F(BarrelShifterInstructionShiftLSLTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $80000001"
    });
    When({
        "ORRS R2, R3, R4, LSL #1"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000002",
        "R3 is $00000000",
        "R4 is $80000001"
    });
}

TEST_F(BarrelShifterInstructionShiftLSLTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000001"
    });
    When({
        "ORRS R2, R3, R4, LSL #1"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000002",
        "R3 is $00000000",
        "R4 is $00000001"
    });
}


