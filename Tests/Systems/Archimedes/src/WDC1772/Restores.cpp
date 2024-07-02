#include "Archimedes/WDC1772Test.h"

class RestoreTest : public WDC1772Test {
public:
    ~RestoreTest() override = default;
};

TEST_F(RestoreTest, RestoreClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(80u);
    WriteCommandRestore({
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

TEST_F(RestoreTest, RestoreWhenInterruptedWillRevertToIdle) {
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandRestore({
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

TEST_F(RestoreTest, RestoreTerminatesImmediatelyWhenThePhsicalHeadIsAlreadyAtTrack00) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(0u);
    WriteCommandRestore({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_EQ(0x00u, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
}

TEST_F(RestoreTest, RestoreMovesTheHeadToTrack00InTheCorrectAmountOfTime) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(20u);
    WriteCommandRestore({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(0xFEu, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    Update(20u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_TRACK_00);
    EXPECT_EQ(0x00u, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
}

TEST_F(RestoreTest, RestoreWithoutVerifyDoesNotSetTheRecordNotFoundFlagWhen255StepsAreRequired) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(256u);
    WriteCommandRestore({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = {},
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    EXPECT_EQ(0xFEu, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    Update(255u * STEPPING_3_MS_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_TRACK_00 | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    EXPECT_EQ(0x00u, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
}

TEST_F(RestoreTest, RestoreWithVerifySetsTheRecordNotFoundFlagWhen255StepsAreRequired) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest()); // Extra call when we read the data register
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_DATA_REQUEST);
    SetHeadPositionTrack(256u);
    WriteCommandRestore({
        .updateTrackRegister = {},
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_EQ(0xFEu, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
    EXPECT_STATUS_SET(STATUS_SPIN_UP);
    Update(255u * STEPPING_3_MS_DELAY);
    EXPECT_EQ(0x00u, ReadRegisterTrack());
    EXPECT_EQ(0x00u, ReadRegisterData());
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_TRACK_00 | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_SPIN_UP | STATUS_BUSY);
    Update(VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
}

TEST_F(RestoreTest, RestoreWithVerifyToNonExistentTrackSetsTheRecordNotFoundBit) {
    {
        testing::InSequence s{};
        EXPECT_CALL(disc, GetTracks())
            .WillOnce(testing::Return(0u));
    }
    SetHeadPositionTrack(0u);
    WriteRegisterTrack(0u);
    WriteCommandRestore({
        .updateTrackRegister = true,
        .motorOn = {},
        .verify = true,
        .stepRate = STEP_RATE_3_MS
    });
    Update(COMMAND_DELAY + VERIFY_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_CRC_ERROR | STATUS_DATA_REQUEST | STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
    EXPECT_EQ(0u, ReadRegisterTrack());
}