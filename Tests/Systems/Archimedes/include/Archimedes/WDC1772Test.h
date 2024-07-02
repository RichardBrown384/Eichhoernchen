#include "Archimedes/WDC1772.h"
#include "Archimedes/ArchimedesMock.h"

#include "Common/Util/BitUtil.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

constexpr auto MilliSeconds(uint32_t v) -> uint32_t { return 1'000'000u * v; }
constexpr auto MicroSeconds(uint32_t v) -> uint32_t { return 1'000u * v; }
constexpr auto NanoSeconds(uint32_t v) -> uint32_t { return v; }

constexpr auto STEP_RATE_3_MS = 3u;

constexpr auto STATUS_MOTOR_ON = 0x80u;
constexpr auto STATUS_WRITE_PROTECT = 0x40u;
constexpr auto STATUS_SPIN_UP = 0x20u;
constexpr auto STATUS_RECORD_NOT_FOUND = 0x10u;
constexpr auto STATUS_CRC_ERROR = 0x08u;
constexpr auto STATUS_TRACK_00 = 0x04u;
constexpr auto STATUS_DATA_REQUEST = 0x02u;
constexpr auto STATUS_BUSY = 0x01u;

constexpr auto STATUS_RECORD_TYPE = 0x20u;
constexpr auto STATUS_LOST_DATA = 0x04u;

constexpr auto INTERRUPT_NONE = 0u;

constexpr auto COMMAND_DELAY = MicroSeconds(24u);
constexpr auto STEPPING_3_MS_DELAY = MilliSeconds(3u);
constexpr auto VERIFY_DELAY = MilliSeconds(15u);
constexpr auto SETTLING_DELAY = MilliSeconds(15u);
constexpr auto BYTE_TRANSFER_DELAY = NanoSeconds(31250u);

constexpr auto ByteGapDelay(uint32_t v) { return 2u * v * BYTE_TRANSFER_DELAY; }

constexpr auto BYTE_GAP_DELAY_DATA_REQUEST = ByteGapDelay(2u);
constexpr auto BYTE_GAP_DELAY_WRITE_GATE = ByteGapDelay(9u);
constexpr auto BYTE_GAP_DELAY_DATA_ADDRESS_MARK = ByteGapDelay(6u);

class DiscImageMock : public rbrown::acorn::archimedes::DiscImage {
public:
    ~DiscImageMock() override = default;

    MOCK_METHOD(bool, IsWriteProtected, (), (const override));
    MOCK_METHOD(uint32_t, GetTracks, (), (const override));
    MOCK_METHOD(uint32_t, GetSectors, (), (const override));
    MOCK_METHOD(uint32_t, GetUserSectorSizeBytes, (), (const override));
    MOCK_METHOD(uint32_t, GetTotalSectorSizeBytes, (), (const override));
    MOCK_METHOD(uint32_t, GetAddressSizeBytes, (), (const override));
    MOCK_METHOD(bool, SeekToData, (uint32_t, uint32_t, uint32_t), (override));
    MOCK_METHOD(uint8_t, ReadByteData, (), (override));
    MOCK_METHOD(void, WriteByteData, (uint8_t), (override));
    MOCK_METHOD(bool, SeekToAddress, (uint32_t, uint32_t, uint32_t), (override));
    MOCK_METHOD(uint8_t, ReadByteAddress, (), (override));
};

struct TypeOneOptions {
    bool updateTrackRegister;
    bool motorOn;
    bool verify;
    uint32_t stepRate;
};

struct TypeTwoOptions {
    bool multipleSectors;
    bool motorOn;
    bool settlingDelay;
    bool deleteMark;
};

struct TypeThreeOptions {
    bool motorOn;
    bool settlingDelay;
};

class WDC1772Test : public testing::Test {
public:
    WDC1772Test();
    ~WDC1772Test() override = default;
    auto Update(uint32_t) -> void;
    auto ReadRegisterStatus() -> uint32_t;
    auto ReadRegisterControl() -> uint32_t;
    auto ReadRegisterTrack() -> uint32_t;
    auto ReadRegisterSector() -> uint32_t;
    auto ReadRegisterData() -> uint32_t;
    auto WriteRegisterStatus(uint8_t) -> void;
    auto WriteRegisterControl(uint8_t) -> void;
    auto WriteRegisterTrack(uint8_t) -> void;
    auto WriteRegisterSector(uint8_t) -> void;
    auto WriteRegisterData(uint8_t) -> void;
    auto TestStatusSet(uint8_t) -> bool;
    auto TestStatusClear(uint8_t) -> bool;
    [[nodiscard]] auto GetHeadPositionTrack() const -> uint32_t;
    [[nodiscard]] auto GetHeadPositionSector() const -> uint32_t;
    auto SetHeadPositionTrack(uint32_t) -> void;
    auto SetHeadPositionSector(uint32_t, uint32_t) -> void;
    auto WriteCommandRestore(const TypeOneOptions&) -> void;
    auto WriteCommandSeek(const TypeOneOptions&) -> void;
    auto WriteCommandStep(const TypeOneOptions&) -> void;
    auto WriteCommandStepIn(const TypeOneOptions&) -> void;
    auto WriteCommandStepOut(const TypeOneOptions&) -> void;
    auto WriteCommandReadSector(const TypeTwoOptions&) -> void;
    auto WriteCommandWriteSector(const TypeTwoOptions&) -> void;
    auto WriteCommandReadAddress(const TypeThreeOptions&) -> void;
    auto WriteCommandForceInterrupt(uint8_t) -> void;
protected:
    testing::NiceMock<ArchimedesMock> mediator;
    testing::NiceMock<DiscImageMock> disc;
private:
    rbrown::acorn::archimedes::WDC1772 wdc1772;
};

#define EXPECT_STATUS_SET(bits) EXPECT_TRUE(TestStatusSet((bits)))
#define EXPECT_STATUS_CLEAR(bits) EXPECT_TRUE(TestStatusClear((bits)))