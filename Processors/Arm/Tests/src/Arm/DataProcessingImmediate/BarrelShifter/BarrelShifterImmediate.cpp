#include "Arm/ArmTest.h"

class BarrelShifterImmediateTest : public ArmTest {};

TEST_F(BarrelShifterImmediateTest, ZeroShiftPreservesCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "ORRS R2, R3, #0x22"
    });
    Then({
        "CYCLES is S",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000022",
        "R3 is $00000000"
    });
}

TEST_F(BarrelShifterImmediateTest, CarryFromBarrelShifterSetsCarryFlag) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "ORRS R2, R3, #0x80000000"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $80000000",
        "R3 is $00000000"
    });
}

TEST_F(BarrelShifterImmediateTest, CarryClearFromBarrelShifterSetsCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000"
    });
    When({
        "ORRS R2, R3, #0x40000000"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $40000000",
        "R3 is $00000000"
    });
}
