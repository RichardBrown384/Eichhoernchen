#include "Archimedes/WDC1772Test.h"

class ForceInterruptTest : public WDC1772Test {
public:
    ~ForceInterruptTest() override = default;
};

TEST_F(ForceInterruptTest, WhenAForceInterruptCommandIsIssuedTheStatusBusyBitShouldThenBeClear) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest())
            .Times(0);
    }
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(ForceInterruptTest, WhenAForceInterruptCommandIsIssuedWhileProcessingACommandThenTheControllerShouldRevertToIdle) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Writing to the data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command setup
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Force interrupt
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest()) // This should not be called
            .Times(0);
    }
    SetHeadPositionTrack(0xC0u);
    WriteRegisterTrack(0xC0u);
    WriteRegisterData(0xA0u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(ForceInterruptTest, WhenAForceInterruptCommandIsIssuedWhileIdleAndTheHeadIsAtTrack00TheStatusRegisterIsSetAppropriately) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest())
            .Times(0);
    }
    SetHeadPositionTrack(0x00u);
    WriteRegisterStatus(0x00u);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    EXPECT_STATUS_CLEAR(STATUS_TRACK_00 | STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_TRACK_00);
}