#include "Archimedes/MemcTest.h"

class ResetTest : public MemcTest {
public:
    ResetTest() : MemcTest(0u) {}
    ~ResetTest() override = default;
};

TEST_F(ResetTest, ResetDisablesOperatingSystemMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    Reset();
    EXPECT_FALSE(GetOperatingSystemMode());
}

TEST_F(ResetTest, ResetDisablesSoundDmaControlAndAssertsTheSoundIrqLine) {
     ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, StartSoundInterrupt);
    Reset();
    EXPECT_FALSE(GetSoundDmaControl());
    EXPECT_FALSE(GetNextSoundBufferValid());
}

TEST_F(ResetTest, ResetDoesNotChangeVideoDmaControl) {
     ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    Reset();
    EXPECT_TRUE(GetVideoDmaControl());
}

TEST_F(ResetTest, ResetDoesNotChangeTheDmaRefreshControl) {
     ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_FLY_BACK,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    Reset();
    EXPECT_EQ(MEMC_DRAM_REFRESH_FLY_BACK, GetDramRefreshControl());
}

TEST_F(ResetTest, ResetSelects450NanoSecondAccessTimeForBothRomRegions) {
     ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_325,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_250,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    Reset();
    EXPECT_EQ(MEMC_HIGH_ROM_ACCESS_450, GetHighRomAccessTime());
    EXPECT_EQ(MEMC_LOW_ROM_ACCESS_450, GetLowRomAccessTime());
}

TEST_F(ResetTest, ResetSelects4KLogicalPageSize) {
     ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_32K
    }));
    Reset();
    EXPECT_EQ(MEMC_PAGE_SIZE_4K, GetPageSize());
}

TEST_F(ResetTest, ResetContinuouslyEnablesRom) {
    Reset();
    EXPECT_TRUE(GetRomContinuouslyEnabled());
}

TEST_F(ResetTest, ResetSetSupervisorMode) {
    Reset();
    EXPECT_TRUE(GetSupervisor());
}

TEST_F(ResetTest, ResetClearsAllDmaRequests) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_ENABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_ENABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_32K
    }));
    StartVideoDmaRequest();
    StartCursorDmaRequest();
    StartSoundDmaRequest();
    EXPECT_EQ(1u, GetVideoDmaRequests());
    EXPECT_EQ(1u, GetCursorDmaRequests());
    EXPECT_EQ(1u, GetSoundDmaRequests());
    Reset();
    EXPECT_EQ(0u, GetVideoDmaRequests());
    EXPECT_EQ(0u, GetCursorDmaRequests());
    EXPECT_EQ(0u, GetSoundDmaRequests());
}