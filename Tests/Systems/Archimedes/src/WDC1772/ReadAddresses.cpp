#include "Archimedes/WDC1772Test.h"

class ReadAddressTest : public WDC1772Test {
public:
    ~ReadAddressTest() override = default;
};

TEST_F(ReadAddressTest, ReadAddressClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(3u));
    }
    WriteRegisterStatus(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_LOST_DATA | STATUS_DATA_REQUEST);
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_RECORD_NOT_FOUND | STATUS_CRC_ERROR | STATUS_LOST_DATA | STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(ReadAddressTest, ReadAddressWhenInterruptedWillRevertToIdle) {
    {
        testing::InSequence s{};
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(3u));
    }
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandReadAddress({});
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(ReadAddressTest, ReadAddressSetsTheRecordNotFoundFlagWhenASectorIsNotFound) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(false));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = {},
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
}

TEST_F(ReadAddressTest, ReadAddressWillCompleteInTheAppropriateAmountOfTime) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        // First five bytes
        for (auto i = 0u; i < 5u; ++i) {
            EXPECT_CALL(disc, ReadByteAddress());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Other events get interleaved with final byte
        EXPECT_CALL(disc, ReadByteAddress());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, GetTotalSectorSizeBytes())
            .WillOnce(testing::Return(2000u));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(6u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = {},
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto i = 0u; i < 6u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    Update((2000u - 6u) * BYTE_TRANSFER_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(6u, ReadRegisterSector());
    EXPECT_EQ(6u, GetHeadPositionTrack());
    EXPECT_EQ(2u, GetHeadPositionSector());
}

TEST_F(ReadAddressTest, ReadAddressWillCorrectlyWrapTheHeadSectorPositionAfterReadingTheFinalSector) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        // First five bytes
        for (auto i = 0u; i < 5u; ++i) {
            EXPECT_CALL(disc, ReadByteAddress());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Other events get interleaved with final byte
        EXPECT_CALL(disc, ReadByteAddress());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, GetTotalSectorSizeBytes())
            .WillOnce(testing::Return(2000u));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(6u);
    SetHeadPositionSector(4u, 5u);
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = {},
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto i = 0u; i < 6u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    Update((2000u - 6u) * BYTE_TRANSFER_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(6u, ReadRegisterSector());
    EXPECT_EQ(6u, GetHeadPositionTrack());
    EXPECT_EQ(0u, GetHeadPositionSector());
}

TEST_F(ReadAddressTest, ReadAddressWithSettlingDelayWillCompleteInTheAppropriateAmountOfTime) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        // First five bytes
        for (auto i = 0u; i < 5u; ++i) {
            EXPECT_CALL(disc, ReadByteAddress());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Other events get interleaved with final byte
        EXPECT_CALL(disc, ReadByteAddress());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, GetTotalSectorSizeBytes())
            .WillOnce(testing::Return(2000u));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(6u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = true,
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(SETTLING_DELAY);
    for (auto i = 0u; i < 6u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    Update((2000u - 6u) * BYTE_TRANSFER_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(6u, ReadRegisterSector());
    EXPECT_EQ(6u, GetHeadPositionTrack());
    EXPECT_EQ(2u, GetHeadPositionSector());
}

TEST_F(ReadAddressTest, ReadAddressWillTheLostDataFlagWhenAReadIsMissed) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToAddress(testing::_, testing::_, testing::_)) // Initial seek
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        // First five bytes
        for (auto i = 0u; i < 5u; ++i) {
            EXPECT_CALL(disc, ReadByteAddress());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            if (i != 2u) {
                EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
            }
        }
        // Other events get interleaved with final byte
        EXPECT_CALL(disc, ReadByteAddress());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, GetTotalSectorSizeBytes())
            .WillOnce(testing::Return(2000u));
        EXPECT_CALL(disc, GetAddressSizeBytes())
            .WillOnce(testing::Return(6u));
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(11u);
    WriteRegisterSector(2u);
    SetHeadPositionTrack(19u);
    SetHeadPositionSector(3u, 5u);
    WriteCommandReadAddress({
        .motorOn = {},
        .settlingDelay = true,
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(SETTLING_DELAY);
    for (auto i = 0u; i < 6u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        if (i != 2u) {
            ReadRegisterData();
        }
    }
    Update((2000u - 6u) * BYTE_TRANSFER_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_LOST_DATA);
    EXPECT_EQ(11u, ReadRegisterTrack());
    EXPECT_EQ(19u, ReadRegisterSector());
    EXPECT_EQ(19u, GetHeadPositionTrack());
    EXPECT_EQ(4u, GetHeadPositionSector());
}