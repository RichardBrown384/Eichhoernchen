#include "Archimedes/WDC1772Test.h"

using namespace rbrown::acorn::archimedes;

class ReadSectorsTest : public WDC1772Test {
public:
    ~ReadSectorsTest() override = default;
};

TEST_F(ReadSectorsTest, ReadSectorClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
    }
    WriteRegisterStatus(
        STATUS_WRITE_PROTECT |
        STATUS_RECORD_TYPE |
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST);
    WriteCommandReadSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_WRITE_PROTECT |
                        STATUS_RECORD_TYPE |
                        STATUS_RECORD_NOT_FOUND |
                        STATUS_CRC_ERROR |
                        STATUS_LOST_DATA |
                        STATUS_DATA_REQUEST);
    EXPECT_STATUS_SET(STATUS_BUSY);
}

TEST_F(ReadSectorsTest, ReadSectorWhenInterruptedWillRevertToIdle) {
    EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(true));
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandReadSector({});
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(ReadSectorsTest, ReadSectorSetsTheRecordNotFoundFlagWhenASectorIsNotFound) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(false));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteCommandReadSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
}

TEST_F(ReadSectorsTest, ReadSectorWillReadA1024ByteSectorInTheAppropriateAmountOfTime) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetUserSectorSizeBytes())
            .WillOnce(testing::Return(1024u));
        for (auto i = 0u; i < 1023u; ++i) {
            EXPECT_CALL(disc, ReadByteData());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Last byte of the sector interleaves the start interrupt request
        EXPECT_CALL(disc, ReadByteData());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandReadSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto i = 0u; i < 1024u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(3u, ReadRegisterSector());
    EXPECT_EQ(10u, GetHeadPositionTrack());
    EXPECT_EQ(4u, GetHeadPositionSector());
}

TEST_F(ReadSectorsTest, ReadSectorWithSettlingDelayWillReadA1024ByteSectorInTheAppropriateAmountOfTime) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetUserSectorSizeBytes())
            .WillOnce(testing::Return(1024u));
        for (auto i = 0u; i < 1023u; ++i) {
            EXPECT_CALL(disc, ReadByteData());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Last byte of the sector interleaves the start interrupt request
        EXPECT_CALL(disc, ReadByteData());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandReadSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = true,
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(SETTLING_DELAY);
    for (auto i = 0u; i < 1024u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(3u, ReadRegisterSector());
    EXPECT_EQ(10u, GetHeadPositionTrack());
    EXPECT_EQ(4u, GetHeadPositionSector());
}

TEST_F(ReadSectorsTest, ReadSectorWillSetTheLostDataFlagWhenAReadIsMissed) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(5u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetUserSectorSizeBytes())
            .WillOnce(testing::Return(1024u));
        for (auto i = 0u; i < 1023u; ++i) {
            EXPECT_CALL(disc, ReadByteData());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            if (i != 200u) {
                EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
            }
        }
        // Last byte of the sector interleaves the start interrupt request
        EXPECT_CALL(disc, ReadByteData());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandReadSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto i = 0u; i < 1024u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        if (i != 200u) {
            ReadRegisterData();
        }
    }
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_LOST_DATA);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(3u, ReadRegisterSector());
    EXPECT_EQ(10u, GetHeadPositionTrack());
    EXPECT_EQ(4u, GetHeadPositionSector());
}

TEST_F(ReadSectorsTest, ReadSectorWillReadMultipleSectorsUntilTheEndOfTheTrack) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(2u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        // First sector
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetUserSectorSizeBytes())
            .WillOnce(testing::Return(128u));
        for (auto i = 0u; i < 127u; ++i) {
            EXPECT_CALL(disc, ReadByteData());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Final byte of the sector is a bit more involved in terms of interactions
        // Byte is placed in the data register and the drive starts immediately setting up the next sector
        EXPECT_CALL(disc, ReadByteData());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(true));
        EXPECT_CALL(disc, GetUserSectorSizeBytes())
            .WillOnce(testing::Return(128u));
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        // Second sector
        for (auto i = 0u; i < 127u; ++i) {
            EXPECT_CALL(disc, ReadByteData());
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        }
        // Final byte of the sector is again more involved
        EXPECT_CALL(disc, ReadByteData());
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(false));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(0u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(0u, 2u);
    WriteCommandReadSector({
        .multipleSectors = true,
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto i = 0u; i < 2u * 128u; ++i) {
        Update(BYTE_TRANSFER_DELAY);
        ReadRegisterData();
    }
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_CRC_ERROR |
        STATUS_LOST_DATA |
        STATUS_DATA_REQUEST |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_RECORD_NOT_FOUND);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(2u, ReadRegisterSector());
    EXPECT_EQ(10u, GetHeadPositionTrack());
    EXPECT_EQ(0u, GetHeadPositionSector());
}
