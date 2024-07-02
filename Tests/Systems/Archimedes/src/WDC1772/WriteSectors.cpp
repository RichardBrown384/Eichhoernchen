#include "Archimedes/WDC1772Test.h"

using namespace rbrown::acorn::archimedes;

class WriteSectorsTest : public WDC1772Test {
public:
    ~WriteSectorsTest() override = default;
};

TEST_F(WriteSectorsTest, WriteSectorClearsTheAppropriateBitsInTheStatusRegisterAndSetsTheBusyFlag) {
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
    WriteCommandWriteSector({
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

TEST_F(WriteSectorsTest, WriteSectorWhenInterruptedWillRevertToIdle) {
    EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(true));
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    WriteCommandWriteSector({});
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    WriteCommandForceInterrupt(INTERRUPT_NONE);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
}

TEST_F(WriteSectorsTest, WriteSectorSetsTheWriteProtectedFlagWhenADiscIsWriteProtected) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, IsWriteProtected())
            .WillOnce(testing::Return(true));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteCommandWriteSector({
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
    EXPECT_STATUS_SET(STATUS_WRITE_PROTECT);
}

TEST_F(WriteSectorsTest, WriteSectorSetsTheRecordNotFoundFlagWhenASectorIsNotFound) {
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, IsWriteProtected())
            .WillOnce(testing::Return(false));
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(false));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteCommandWriteSector({
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

TEST_F(WriteSectorsTest, WriteSectorWillWriteA1024ByteSectorInTheAppropriateAmountOfTime) {
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
        for (auto i = 0u; i < 1024u; ++i) {
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
            EXPECT_CALL(disc, WriteByteData(testing::_));
        }
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandWriteSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(BYTE_GAP_DELAY_DATA_REQUEST);
    WriteRegisterData(0xA5u);
    Update(BYTE_GAP_DELAY_WRITE_GATE);
    Update(BYTE_GAP_DELAY_DATA_ADDRESS_MARK);
    for (auto i = 0u; i < 1023u; ++i) {
        WriteRegisterData(0x22u);
        Update(BYTE_TRANSFER_DELAY);
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

TEST_F(WriteSectorsTest, WriteSectorWithSettlingDelayWillWriteA1024ByteSectorInTheAppropriateAmountOfTime) {
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
        for (auto i = 0u; i < 1024u; ++i) {
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
            EXPECT_CALL(disc, WriteByteData(testing::_));
        }
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandWriteSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = true,
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(SETTLING_DELAY);
    Update(BYTE_GAP_DELAY_DATA_REQUEST);
    WriteRegisterData(0xA5u);
    Update(BYTE_GAP_DELAY_WRITE_GATE);
    Update(BYTE_GAP_DELAY_DATA_ADDRESS_MARK);
    for (auto i = 0u; i < 1023u; ++i) {
        WriteRegisterData(0x22u);
        Update(BYTE_TRANSFER_DELAY);
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

TEST_F(WriteSectorsTest, WriteSectorWillSetTheLostDataFlagAndInteruptWhenTheFirstByteIsNotWrittenInTime) {
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
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest())
            .Times(0);
        EXPECT_CALL(disc, WriteByteData(testing::_))
            .Times(0);
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandWriteSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = true,
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(SETTLING_DELAY);
    Update(BYTE_GAP_DELAY_DATA_REQUEST);
    Update(BYTE_GAP_DELAY_WRITE_GATE);
    EXPECT_STATUS_CLEAR(
        STATUS_RECORD_TYPE |
        STATUS_RECORD_NOT_FOUND |
        STATUS_CRC_ERROR |
        STATUS_BUSY);
    EXPECT_STATUS_SET(STATUS_LOST_DATA | STATUS_DATA_REQUEST);
    EXPECT_EQ(10u, ReadRegisterTrack());
    EXPECT_EQ(3u, ReadRegisterSector());
    EXPECT_EQ(10u, GetHeadPositionTrack());
    EXPECT_EQ(4u, GetHeadPositionSector());
}

TEST_F(WriteSectorsTest, WriteSectorWillWriteZeroToDiscWhenAByteIsNotWrittenInTime) {
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
        // First write
        EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        EXPECT_CALL(disc, WriteByteData(testing::_));
        // Subsequent writes
        for (auto i = 0u; i < 1023u; ++i) {
            EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
            if (i == 1000u) {
                EXPECT_CALL(disc, WriteByteData(0u));
            } else {
                EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
                EXPECT_CALL(disc, WriteByteData(testing::_));
            }
        }
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(3u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(1u, 5u);
    WriteCommandWriteSector({
        .multipleSectors = {},
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    Update(BYTE_GAP_DELAY_DATA_REQUEST);
    // First write
    WriteRegisterData(0xA5u);
    Update(BYTE_GAP_DELAY_WRITE_GATE);
    Update(BYTE_GAP_DELAY_DATA_ADDRESS_MARK);
    // Subsequent writes
    for (auto i = 0u; i < 1023u; ++i) {
        if (i != 1000u) {
            WriteRegisterData(0x22u);
        }
        Update(BYTE_TRANSFER_DELAY);
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

TEST_F(WriteSectorsTest, WriteSectorWillWriteMultipleSectorsUntilTheEndOfTheTrack) {
    EXPECT_CALL(disc, GetSectors())
        .WillRepeatedly(testing::Return(2u));
    {
        testing::InSequence s{};
        EXPECT_CALL(mediator, EndFloppyDiscInterruptRequest()); // Initial command
        EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
        for (auto j = 0u; j < 2u; ++j) {
            EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
                .WillOnce(testing::Return(true));
            EXPECT_CALL(disc, GetUserSectorSizeBytes())
                .WillOnce(testing::Return(128u));
            for (auto i = 0u; i < 128u; ++i) {
                EXPECT_CALL(mediator, StartFloppyDiscDataRequest());
                EXPECT_CALL(mediator, EndFloppyDiscDataRequest());
                EXPECT_CALL(disc, WriteByteData(testing::_));
            }
        }
        EXPECT_CALL(disc, SeekToData(testing::_, testing::_, testing::_))
            .WillOnce(testing::Return(false));
        EXPECT_CALL(mediator, StartFloppyDiscInterruptRequest());
    }
    WriteRegisterTrack(10u);
    WriteRegisterSector(0u);
    SetHeadPositionTrack(10u);
    SetHeadPositionSector(0u, 2u);
    WriteCommandWriteSector({
        .multipleSectors = true,
        .motorOn = {},
        .settlingDelay = {},
        .deleteMark = {}
    });
    EXPECT_STATUS_CLEAR(STATUS_BUSY);
    Update(COMMAND_DELAY);
    EXPECT_STATUS_SET(STATUS_BUSY);
    for (auto j = 0u; j < 2; ++j) {
        Update(BYTE_GAP_DELAY_DATA_REQUEST);
        WriteRegisterData(0xA5u);
        Update(BYTE_GAP_DELAY_WRITE_GATE);
        Update(BYTE_GAP_DELAY_DATA_ADDRESS_MARK);
        for (auto i = 0u; i < 127u; ++i) {
            WriteRegisterData(0x22u);
            Update(BYTE_TRANSFER_DELAY);
        }
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