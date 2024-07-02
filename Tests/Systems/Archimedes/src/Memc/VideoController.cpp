#include "Archimedes/MemcTest.h"

class VideoControllerTest: public MemcTest {
public:
    VideoControllerTest() : MemcTest(0u) {}
    ~VideoControllerTest() override = default;
};

TEST_F(VideoControllerTest, VideoControllerIsAccessibleInSupervisorMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, WriteWordVidcRegister(0xDDCCEEAAu));
    EXPECT_TRUE(WriteWordVideoController(0u, 0xDDCCEEAAu));
}

TEST_F(VideoControllerTest, VideoControllerIsNotAccessibleInOperatingSystemMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    SetSupervisor(false);
    EXPECT_CALL(mediator, WriteWordVidcRegister)
        .Times(0u);
    EXPECT_FALSE(WriteWordVideoController(0u, 0xDDCCEEAAu));
}

TEST_F(VideoControllerTest, VideoControllerIsNotAccessibleInUserModeMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    SetSupervisor(false);
    EXPECT_CALL(mediator, WriteWordVidcRegister)
        .Times(0u);
    EXPECT_FALSE(WriteWordVideoController(0u, 0xDDCCEEAAu));
}

TEST_F(VideoControllerTest, VideoControllerIsAccessibleInSupervisorModeWithByteAccess) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, WriteWordVidcRegister(0xCDCDCDCDu));
    EXPECT_TRUE(WriteByteVideoController(0u, 0xCDu));
}