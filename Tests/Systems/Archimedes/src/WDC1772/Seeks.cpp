#include "Archimedes/WDC1772Test.h"

class SeekTest : public WDC1772Test {
public:
    ~SeekTest() override = default;
};

TEST_F(SeekTest, SeekClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
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
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(SeekTest, SeekWhenInterruptedWillRevertToIdle) {
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandSeek({
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

TEST_F(SeekTest, SeekTerminatesImmediatelyWhenTheTrackAndDataRegistersAreEqual) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final data register read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(10u);
    WriteRegisterTrack(10u);
    WriteRegisterData(10u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(SeekTest, SeekingOutMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(2u); // Reads during operation
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final data register read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandSeek({
        .updateTrackRegister = {},
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
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(18u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(9u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(SeekTest, SeekingOutWithVerifyMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(3u); // Reads during operation
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandSeek({
        .updateTrackRegister = {},
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
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(18u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(9u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(SeekTest, SeekingOutWhenTheHeadIsAtTrack00CompletesInTheRightAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(0u);
    WriteRegisterTrack(20u);
    WriteRegisterData(10u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_TRACK_00);
    EXPECT_EQ(0u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(SeekTest, SeekingInMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(2u); // Reads during operation
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final read
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(30u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(21u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(22u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    Update(9u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(30u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
}

TEST_F(SeekTest, SeekingInWithVerifyMovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(3u); // Reads during operation
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteRegisterTrack(20u);
    WriteRegisterData(30u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(21u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(22u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    Update(9u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_BUSY);
    EXPECT_EQ(30u, ReadRegisterTrack());
    EXPECT_EQ(30u, ReadRegisterData());
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(SeekTest, SeekingInStartingFromTrack00MovesTheHeadInTheAppropriateDirectionAndCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Write to data register
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Command set up
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(2u); // calls during operation
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Final call
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(0u);
    WriteRegisterTrack(0u);
    WriteRegisterData(10u);
    WriteCommandSeek({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(1u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(2u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
    Update(9u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(10u, ReadRegisterData());
}

TEST_F(SeekTest, SeekWithVerifyToNonExistentTrackSetsTheRecordNotFoundBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(disc, GetTracks())
            .WillOnce(testing::Return(0u));
    }
    SetHeadPositionTrack(0u);
    WriteRegisterTrack(0u);
    WriteRegisterData(1u);
    WriteCommandSeek({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + STEPPING_3_MS_DELAY + VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
    EXPECT_EQ(1u, ReadRegisterTrack());
}