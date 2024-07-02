#include "Arm/ArmTest.h"

class BarrelShifterRegisterShiftASRTest : public ArmTest {};

TEST_F(BarrelShifterRegisterShiftASRTest, ZeroShiftPropagatesCarryFlag) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000100"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is C,SVC",
        "PC is $0000100C",
        "R2 is $00000003",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000100"
    });
}

TEST_F(BarrelShifterRegisterShiftASRTest, NonZeroShiftSetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000001",
        "R5 is $00000101"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
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

TEST_F(BarrelShifterRegisterShiftASRTest, NonZeroShiftClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $00000002",
        "R5 is $00000101"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
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

TEST_F(BarrelShifterRegisterShiftASRTest, Shift32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftASRTest, Shift32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $7FFFFFFF",
        "R5 is $00000120"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $7FFFFFFF",
        "R5 is $00000120"
    });
}

TEST_F(BarrelShifterRegisterShiftASRTest, ShiftMoreThan32ClearsCarry) {
    Given({
        "PSR is C,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $40000003",
        "R5 is $00000123"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is 0,SVC",
        "PC is $0000100C",
        "R2 is $00000001",
        "R3 is $00000001",
        "R4 is $40000003",
        "R5 is $00000123"
    });
}

TEST_F(BarrelShifterRegisterShiftASRTest, ShiftMoreThan32SetsCarry) {
    Given({
        "PSR is 0,SVC",
        "PC is $00001008",
        "R2 is $00000000",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000123"
    });
    When({
        "ORRS R2, R3, R4, ASR R5"
    });
    Then({
        "CYCLES is SS",
        "PSR is NC,SVC",
        "PC is $0000100C",
        "R2 is $FFFFFFFF",
        "R3 is $00000001",
        "R4 is $80000003",
        "R5 is $00000123"
    });
}