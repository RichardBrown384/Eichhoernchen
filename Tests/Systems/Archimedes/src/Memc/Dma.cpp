#include "Archimedes/MemcTest.h"

/*
 * Note: We use MEMC_PAGE_SIZE_8K here because the page size is
 * encoded in address line bits 3 and 2 and MEMC_PAGE_SIZE_8K is non-zero.
 * A zero value here (MEMC_PAGE_SIZE_4K) would make the MEMC force an
 * N-cycle when we want a S-cycle.
 *
 */

class DmaTest : public MemcTest {
public:
    DmaTest() : MemcTest(ONE_MEGABYTE) {}
    ~DmaTest() override = default;
};

constexpr std::array DMA_DATA = { 0xFFEEDDCCu, 0xBBAA9988u, 0x77665544u, 0x33221100u };

constexpr auto PhysicalDmaAddress(uint32_t pointer, uint32_t word) -> uint32_t {
    return (pointer << 4u) + ((word & 3u) << 2u);
}

TEST_F(DmaTest, WhenSoundDmaIsEnabledSoundDmaRequestsAreAcknowledged) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    StartSoundDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(2u, GetSoundDmaRequests());
}

TEST_F(DmaTest, WhenSoundDmaIsDisabledSoundDmaRequestsAreNotAcknowledged) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    StartSoundDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(0u, GetSoundDmaRequests());
}

TEST_F(DmaTest, WhenSoundDmaIsDisabledExistingSoundDmaRequestsAreCancelled) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
    StartVideoDmaRequest();
    StartCursorDmaRequest();
    StartSoundDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EXPECT_EQ(2u, GetSoundDmaRequests());
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
}

TEST_F(DmaTest, EndingSoundDmaRequestsDecrementsTheRequestCounter) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    StartSoundDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(2u, GetSoundDmaRequests());
    EndSoundDmaRequest();
    EXPECT_EQ(1u, GetSoundDmaRequests());
    EndSoundDmaRequest();
    EXPECT_EQ(0u, GetSoundDmaRequests());
    EndSoundDmaRequest();
    EXPECT_EQ(0u, GetSoundDmaRequests());
}

TEST_F(DmaTest, WhenVideoDmaIsEnabledVideoAndCursorDmaRequestsAreAcknowledged) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    StartVideoDmaRequest();
    StartVideoDmaRequest();
    StartCursorDmaRequest();
    StartCursorDmaRequest();
    StartCursorDmaRequest();
    EXPECT_EQ(2u, GetVideoDmaRequests());
    EXPECT_EQ(3u, GetCursorDmaRequests());
}

TEST_F(DmaTest, WhenVideoDmaIsDisabledVideoAndCursorDmaRequestsAreNotAcknowledged) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    StartVideoDmaRequest();
    StartCursorDmaRequest();
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
}

TEST_F(DmaTest, WhenVideoDmaIsDisabledExistingVideoDmaRequestsAreCancelled) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
    StartVideoDmaRequest();
    StartCursorDmaRequest();
    StartSoundDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EXPECT_EQ(2u, GetSoundDmaRequests());
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(2u, GetSoundDmaRequests());
}

TEST_F(DmaTest, EndingVideoDmaRequestsDecrementsTheRequestCounter) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    StartVideoDmaRequest();
    StartVideoDmaRequest();
    EXPECT_EQ(2u, GetVideoDmaRequests());
    EndVideoDmaRequest();
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EndVideoDmaRequest();
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EndVideoDmaRequest();
    EXPECT_EQ(0u, GetVideoDmaRequests());
}

TEST_F(DmaTest, EndingCursorDmaRequestsDecrementsTheRequestCounter) {
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_EQ(0u, GetCursorDmaRequests());
    StartCursorDmaRequest();
    StartCursorDmaRequest();
    EXPECT_EQ(2u, GetCursorDmaRequests());
    EndCursorDmaRequest();
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EndCursorDmaRequest();
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EndCursorDmaRequest();
    EXPECT_EQ(0u, GetCursorDmaRequests());
}

TEST_F(DmaTest, WhenVideoDmaIsEnabledARequestIsServicedInAnNCycle) {
    const auto videoDmaStart = 0xFFu;
    const auto videoDmaInit = 0x1FFu;
    const auto videoDmaEnd = 0x2FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcVideo(DMA_DATA[0u]));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcVideo(DMA_DATA[1u + i]));
        }
        EXPECT_CALL(mediator, EndVideoDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    for (auto i = 0u; i < 4u; ++i) {
        EXPECT_TRUE(WriteWordPhysical(PhysicalDmaAddress(videoDmaInit, i), DMA_DATA[i]));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = videoDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = videoDmaInit
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = videoDmaEnd
    }));
    EndFlyBack();
    StartVideoDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(1u + videoDmaInit, GetVideoPointer());
}

TEST_F(DmaTest, WhenTheEndOfTheVideoBufferIsReachedTheVideoPointerMovesToVideoStart) {
    const auto videoDmaStart = 0xFFu;
    const auto videoDmaInit = 0x2FFu;
    const auto videoDmaEnd = 0x2FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcVideo);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcVideo);
        }
        EXPECT_CALL(mediator, EndVideoDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = videoDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = videoDmaInit
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = videoDmaEnd
    }));
    EndFlyBack();
    StartVideoDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(videoDmaStart, GetVideoPointer());
}

TEST_F(DmaTest, WhenServicingVideoDmaTriggersAnotherVideoDmaRequestBothAreServiced) {
    const auto videoDmaStart = 0xFFu;
    const auto videoDmaInit = 0x1FFu;
    const auto videoDmaEnd = 0x2FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcVideo)
            .WillOnce(testing::Invoke([this]() -> void { StartVideoDmaRequest(); }));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcVideo);
        }
        EXPECT_CALL(mediator, EndVideoDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcVideo);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcVideo);
        }
        EXPECT_CALL(mediator, EndVideoDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = videoDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = videoDmaInit
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = videoDmaEnd
    }));
    EndFlyBack();
    StartVideoDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(2u + videoDmaInit, GetVideoPointer());
}

TEST_F(DmaTest, WhenVideoDmaIsEnabledARequestIsNotServicedInASCycle) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, WriteWordVidcVideo)
            .Times(0u);
        EXPECT_CALL(mediator, EndVideoDmaRequest)
            .Times(0u);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    const auto videoPointer = GetVideoPointer();
    StartVideoDmaRequest();
    uint32_t discard;
    CycleS();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EXPECT_EQ(videoPointer, GetVideoPointer());
}

TEST_F(DmaTest, WhenCursorDmaIsEnabledARequestIsServicedInAnNCycle) {
    const auto cursorDmaInit = 0x1FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcCursor(DMA_DATA[0u]));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcCursor(DMA_DATA[1u + i]));
        }
        EXPECT_CALL(mediator, EndCursorDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    for (auto i = 0u; i < 4u; ++i) {
        EXPECT_TRUE(WriteWordPhysical(PhysicalDmaAddress(cursorDmaInit, i), DMA_DATA[i]));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = cursorDmaInit
    }));
    EndFlyBack();
    StartCursorDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(1u + cursorDmaInit, GetCursorPointer());
}

TEST_F(DmaTest, WhenServicingCursorDmaTriggersAnotherCursorDmaRequestBothAreServiced) {
    const auto cursorDmaInit = 0x1FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcCursor)
            .WillOnce(testing::Invoke([this]() -> void { StartCursorDmaRequest(); }));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcCursor);
        }
        EXPECT_CALL(mediator, EndCursorDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcCursor);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcCursor);
        }
        EXPECT_CALL(mediator, EndCursorDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = cursorDmaInit
    }));
    EndFlyBack();
    StartCursorDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(2u + cursorDmaInit, GetCursorPointer());
}

TEST_F(DmaTest, WhenCursorDmaIsEnabledARequestIsNotServicedInASCycle) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, WriteWordVidcCursor)
            .Times(0u);
        EXPECT_CALL(mediator, EndCursorDmaRequest)
            .Times(0u);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    const auto cursorPointer = GetCursorPointer();
    StartCursorDmaRequest();
    uint32_t discard;
    CycleS();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EXPECT_EQ(cursorPointer, GetCursorPointer());
}

TEST_F(DmaTest, WhenSoundDmaIsEnabledARequestIsServicedInAnNCycle) {
    const auto soundDmaStart = 0x0Fu;
    const auto soundDmaEnd = 0xFFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound(DMA_DATA[0u]));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound(DMA_DATA[1u + i]));
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    for (auto i = 0u; i < 4u; ++i) {
        EXPECT_TRUE(WriteWordPhysical(PhysicalDmaAddress(soundDmaStart, i), DMA_DATA[i]));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = soundDmaEnd
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = soundDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0xFFFFu
    }));
    StartSoundDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    EXPECT_EQ(1u + soundDmaStart, GetSoundPointer());
}

TEST_F(DmaTest, WhenTheEndOfTheSoundBufferIsReachedAndTheNextBufferIsInvalidTheSoundPointerMovesBackToStart) {
    const auto soundDmaStart = 0xFFu;
    const auto soundDmaEnd = 0xFFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = soundDmaEnd
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = soundDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0xFFFFu
    }));
    EXPECT_FALSE(GetNextSoundBufferValid());
    StartSoundDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    EXPECT_EQ(soundDmaStart, GetSoundPointer());
    EXPECT_EQ(soundDmaEnd, GetSoundEndCurrent());
}

TEST_F(DmaTest, WhenTheEndOfTheSoundBufferIsReachedAndTheNextBufferIsValidTheSoundPointerMovesToTheNewBuffer) {
    const auto soundDmaStart = 0xFFu;
    const auto soundDmaEnd = 0xFFu;
    const auto soundDmaStartNext = 0x2FFu;
    const auto soundDmaEndNext = 0x3FFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, StartSoundInterrupt);
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(2);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, StartSoundInterrupt);
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = soundDmaEnd
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = soundDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0xFFFFu
    }));
    EXPECT_FALSE(GetNextSoundBufferValid());
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = soundDmaEndNext
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = soundDmaStartNext
    }));
    EXPECT_TRUE(GetNextSoundBufferValid());
    StartSoundDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    EXPECT_EQ(soundDmaStartNext, GetSoundPointer());
    EXPECT_EQ(soundDmaEnd, GetSoundEndNext());
    EXPECT_EQ(soundDmaEndNext, GetSoundEndCurrent());
}

TEST_F(DmaTest, WhenServicingSoundDmaTriggersAnotherSoundDmaRequestBothAreServiced) {
    const auto soundDmaStart = 0x0Fu;
    const auto soundDmaEnd = 0xFFu;
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
         EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(3);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound)
            .WillOnce(testing::Invoke([this]() -> void { StartSoundDmaRequest(); }));
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = soundDmaEnd
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = soundDmaStart
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0xFFFFu
    }));
    StartSoundDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetSoundDmaRequests());
    EXPECT_EQ(2u + soundDmaStart, GetSoundPointer());
}

TEST_F(DmaTest, WhenSoundDmaIsEnabledARequestIsNotServicedInASCycle) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, WriteWordVidcSound)
            .Times(0u);
        EXPECT_CALL(mediator, EndSoundDmaRequest)
            .Times(0u);
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    const auto soundPointer = GetSoundPointer();
    StartSoundDmaRequest();
    uint32_t discard;
    CycleS();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(1u, GetSoundDmaRequests());
    EXPECT_EQ(soundPointer, GetSoundPointer());
}

TEST_F(DmaTest, VideoDmaTakesPriorityOverSoundDmaAndBothRequestsAreServiced) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcVideo);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcVideo);
        }
        EXPECT_CALL(mediator, EndVideoDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    StartSoundDmaRequest();
    StartVideoDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
}

TEST_F(DmaTest, CursorDmaTakesPriorityOverSoundDmaAndBothRequestsAreServiced) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcCursor);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcCursor);
        }
        EXPECT_CALL(mediator, EndCursorDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS));
        EXPECT_CALL(mediator, WriteWordVidcSound);
        for (auto i = 0; i < 3; ++i) {
            EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS));
            EXPECT_CALL(mediator, WriteWordVidcSound);
        }
        EXPECT_CALL(mediator, EndSoundDmaRequest);
        EXPECT_CALL(mediator, Update(MEMC_ROM_CYCLE_450_NS));
    }
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    StartSoundDmaRequest();
    StartCursorDmaRequest();
    uint32_t discard;
    CycleN();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
}

TEST_F(DmaTest, ReadsFromAllMemoryRegionsCanTriggerDmaOperations) {
    EXPECT_CALL(mediator, WriteWordVidcVideo)
        .Times(10u * 4u);
    EXPECT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    }));
    uint32_t discard;
    CycleN();
    // Logical
    StartVideoDmaRequest();
    EXPECT_FALSE(ReadWordLogical(0xCu, discard));
    StartVideoDmaRequest();
    EXPECT_FALSE(ReadByteLogical(0xCu, discard));
    // Physical
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadWordPhysical(0xCu, discard));
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadBytePhysical(0xCu, discard));
    // I/O
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadWordInputOutputController(0xCu, discard));
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadByteInputOutputController(0xCu, discard));
    // Low rom
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadWordLowRom(0xCu, discard));
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadByteLowRom(0xCu, discard));
    // High Rom
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadWordHighRom(0xCu, discard));
    StartVideoDmaRequest();
    EXPECT_TRUE(ReadByteHighRom(0xCu, discard));
    // Teardown
    EXPECT_EQ(0u, GetVideoDmaRequests());
}

TEST_F(DmaTest, WritesFromAllMemoryRegionsCanTriggerDmaOperations) {
    EXPECT_CALL(mediator, WriteWordVidcVideo)
        .Times(10u * 4u);
    const ControlWord word = {
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_8K
    };
    EXPECT_TRUE(WriteWordMemcControl(word));
    CycleN();
    // Logical
    StartVideoDmaRequest();
    EXPECT_FALSE(WriteWordLogical(0xCu, 0u));
    StartVideoDmaRequest();
    EXPECT_FALSE(WriteByteLogical(0xCu, 0u));
    // Physical
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordPhysical(0xCu, 0u));
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteBytePhysical(0xCu, 0u));
    // I/O
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordInputOutputController(0xCu, 0u));
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteByteInputOutputController(0xCu, 0u));
    // Vidc (missing byte access)
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordVideoController(0xCu, 0u));
    // Memc control register (missing byte access)
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordMemcControl(word));
    // Memc dma address generator (missing byte access)
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0u
    }));
    // Memc address translator (missing byte access)
    StartVideoDmaRequest();
    EXPECT_TRUE(WriteWordAddressTranslator({}));
    // Teardown
    EXPECT_EQ(0u, GetVideoDmaRequests());
}