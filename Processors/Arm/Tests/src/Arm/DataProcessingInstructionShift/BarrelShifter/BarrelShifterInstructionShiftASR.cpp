#include "Arm/ArmTest.h"

class BarrelShifterInstructionShiftASRTest : public ArmTest {};

TEST_F(BarrelShifterInstructionShiftASRTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000003"
    });
    When({
        "ORRS R2, R3, R4, ASR #1"
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

TEST_F(BarrelShifterInstructionShiftASRTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $00000002"
    });
    When({
        "ORRS R2, R3, R4, ASR #1"
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

TEST_F(BarrelShifterInstructionShiftASRTest, NonZeroShiftPropagatesBit31WhenSet) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $80000002"
    });
    When({
        "ORRS R2, R3, R4, ASR #1"
    });
    Then({
        "CYCLES is S",
        "PSR is N,SVC",
        "PC is $0000100C",
        "R2 is $C0000001",
        "R3 is $00000000",
        "R4 is $80000002"
    });
}

TEST_F(BarrelShifterInstructionShiftASRTest, NonZeroShiftPropagatesBit31WhenClear) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $40000002"
    });
    When({
        "ORRS R2, R3, R4, ASR #1"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $20000001",
        "R3 is $00000000",
        "R4 is $40000002"
    });
}

TEST_F(BarrelShifterInstructionShiftASRTest, Shift32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000000",
        "R4 is $80000000"
    });
    When({
        "ORRS R2, R3, R4, ASR #32"
    });
    Then({
        "CYCLES is S",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000000",
        "R4 is $80000000"
    });
}

TEST_F(BarrelShifterInstructionShiftASRTest, Shift32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00001000",
        "R3 is $00001001",
        "R4 is $00000000"
    });
    When({
        "ORRS R2, R3, R4, ASR #32"
    });
    Then({
        "CYCLES is S",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00001001",
        "R3 is $00001001",
        "R4 is $00000000"
    });
}