#include "Arm/ArmTest.h"

class BarrelShifterInstructionShiftRRXTest : public ArmTest {};

TEST_F(BarrelShifterInstructionShiftRRXTest, SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00001000",
        "R3 is $00001000",
        "R4 is $80000001"
    });
    When({
        "ORRS R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $40001000",
        "R3 is $00001000",
        "R4 is $80000001"
    });
}

TEST_F(BarrelShifterInstructionShiftRRXTest, ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00001000",
        "R3 is $00001000",
        "R4 is $80000000"
    });
    When({
        "ORRS R2, R3, R4, RRX"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $C0001000",
        "R3 is $00001000",
        "R4 is $80000000"
    });
}
