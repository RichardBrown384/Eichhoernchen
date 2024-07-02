#include "Archimedes/WDC1772Test.h"

class RegisterTest : public WDC1772Test {
public:
    ~RegisterTest() override = default;
};

TEST_F(RegisterTest, ReadingTheStatusRegisterEndsTheInterruptRequestAndPreservesTheBusyBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_BUSY);
    ReadRegisterStatus();
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(RegisterTest, ReadingTheDataRegisterEndsTheDataRequestAndClearsTheDataRequestBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterStatus(STATUS_DATA_REQUEST | STATUS_BUSY);
    ReadRegisterData();
    EXPECT_STATUS_CLEAR(STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(RegisterTest, WritingToTheDataRegisterWhileIdleEndsTheDataRequestAndClearsTheDataRequestBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterStatus(STATUS_DATA_REQUEST);
    WriteRegisterData(1u);
    EXPECT_STATUS_CLEAR(STATUS_DATA_REQUEST);
}

TEST_F(RegisterTest, WritingToTheDataRegisterWhileBusyEndsTheDataRequestAndClearsTheDataRequestBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // initial command setup
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // read from the status register to manipulate it
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // data register read
    }
    WriteCommandWriteSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = true,
        .deleteMark = {}
    });
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    // FDC is now busy and artificially force data request high
    WriteRegisterStatus(ReadRegisterStatus() | STATUS_DATA_REQUEST);
    WriteRegisterData(1u);
    EXPECT_STATUS_CLEAR(STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(RegisterTest, WritingToTheControlRegisterWhileIdleEndsTheCurrentInterruptAndDataRequests) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteCommandRestore({});
}

TEST_F(RegisterTest, WritingToTheControlRegisterWhileBusyHasNoEffectForNormalCommands) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    SetHeadPositionTrack(80u);
    WriteCommandRestore({});
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    const auto controlValue = ReadRegisterControl();
    WriteCommandStepOut({});
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(controlValue, ReadRegisterControl());
}

TEST_F(RegisterTest, WritingToTheTrackAndSectorRegistersWhileBusyHasNoEffect) {
    SetHeadPositionTrack(80u);
    WriteRegisterTrack(18);
    WriteRegisterSector(13);
    WriteRegisterData(30u);
    WriteCommandStepOut({
        .updateTrackRegister = false,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteRegisterTrack(99);
    WriteRegisterSector(88);
    EXPECT_EQ(18, ReadRegisterTrack());
    EXPECT_EQ(13, ReadRegisterSector());
}