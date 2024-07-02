#include "Archimedes/WDC1772Test.h"

class StepTest : public WDC1772Test {
public:
    ~StepTest() override = default;
};

TEST_F(StepTest, StepClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
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
    WriteCommandStep({
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

TEST_F(StepTest, StepWhenInterruptedWillRevertToIdle) {
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandStep({
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

TEST_F(StepTest, StepAfterRestoreMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Data read after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final data read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(0u);
    WriteCommandRestore({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(0u, ReadRegisterTrack());
    EXPECT_EQ(0u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(0u, ReadRegisterTrack());
    EXPECT_EQ(0u, ReadRegisterData());
}

TEST_F(StepTest, StepAfterSeekOutMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read after initial command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read after initial command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(30u);
    WriteCommandSeek({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + 3u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(30u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(29u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
}

TEST_F(StepTest, StepAfterSeekInMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read after second command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandSeek({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + 3u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(36u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(37u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepAfterStepOutMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read after second command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(32u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(31u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepAfterStepInMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read after second command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepIn({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(34u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(35u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepsAfterStepInMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data to register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // First command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after second command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Third command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after third command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepIn({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(34u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(35u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(36u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepsAfterStepOutMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data to register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // First command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after second command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Third command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after third command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepOut({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(32u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(31u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
     WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(30u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepWithoutUpdateMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTimeAndPreservesTrackRegister) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data to register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // First command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after second command
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepIn({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(34u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(35u, GetHeadPositionTrack());
    EXPECT_EQ(34u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
}

TEST_F(StepTest, StepWithVerifyMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data to register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // First command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data after first command
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Second command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Read data during second command
        EXPECT_CALL(disc, GetTracks())
            .WillOnce(testing::Return(80u));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteRegisterData(36u);
    WriteCommandStepIn({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(34u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_BUSY);
    EXPECT_EQ(35u, ReadRegisterTrack());
    EXPECT_EQ(36u, ReadRegisterData());
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
}

TEST_F(StepTest, StepWithVerifyToNonExistentTrackSetsTheRecordNotFoundBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(disc, GetTracks())
            .WillOnce(testing::Return(20u));
    }
    SetHeadPositionTrack(33u);
    WriteRegisterTrack(33u);
    WriteCommandStepIn({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(34u, ReadRegisterTrack());
    WriteCommandStep({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_BUSY);
    EXPECT_EQ(35u, ReadRegisterTrack());
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_RECORD_NOT_FOUND);
}
