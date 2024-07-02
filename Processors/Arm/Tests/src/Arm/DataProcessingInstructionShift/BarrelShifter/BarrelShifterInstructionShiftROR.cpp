#include "Arm/ArmTest.h"

class BarrelShifterInstructionShiftRORTest : public ArmTest {};

TEST_F(BarrelShifterInstructionShiftRORTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00010000",
        "R3 is $00010000",
        "R4 is $00000010"
    });
    When({
        "ORRS R2, R3, R4, ROR #5"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80010000",
        "R3 is $00010000",
        "R4 is $00000010"
    });
}

TEST_F(BarrelShifterInstructionShiftRORTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00010000",
        "R3 is $00010000",
        "R4 is $00000020"
    });
    When({
        "ORRS R2, R3, R4, ROR #5"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00010001",
        "R3 is $00010000",
        "R4 is $00000020"
    });
}
