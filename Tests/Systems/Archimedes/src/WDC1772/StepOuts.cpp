#include "Archimedes/WDC1772Test.h"

class StepOutTest : public WDC1772Test {
public:
    ~StepOutTest() override = default;
};

TEST_F(StepOutTest, StepOutClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(StepOutTest, StepOutWhenInterruptedWillRevertToIdle) {
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandStepOut({
        .updateTrackRegister = {},
        .motorOn = true,
        .verify = {},
        .stepRate = {}
    });
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(StepOutTest, StepOutMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Data read during execution
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final Data Read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(19u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(19u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(StepOutTest, StepOutWithoutUpdateMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Data read during execution
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final Data Read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandStepOut({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(20u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(19u, GetHeadPositionTrack());
    EXPECT_EQ(20u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(StepOutTest, StepOutWithVerifyMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Data read during execution
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(19u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
}

TEST_F(StepOutTest, StepOutWhenTheHeadIsAtTrack00CompletesInTheRightAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final Data Read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(0u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandStepOut({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(0u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(StepOutTest, StepOutWithVerifyToNonExistentTrackSetsTheRecordNotFoundBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(disc, GetTracks())
            .WillOnce(testing::Return(15u));
    }
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY + VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
    EXPECT_EQ(19u, ReadRegisterTrack());
}