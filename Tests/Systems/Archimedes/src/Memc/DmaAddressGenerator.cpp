#include "Archimedes/MemcTest.h"

class DmaAddressGeneratorTest : public MemcTest {
public:
    DmaAddressGeneratorTest(): MemcTest(0u) {}
    ~DmaAddressGeneratorTest() override = default;
};

TEST_F(DmaAddressGeneratorTest, WritingToADmaAddressRegisterDoesNotChangeTheControlRegister) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x7FFFu
    }));
    EXPECT_EQ(MEMC_OS_MODE_DISABLE, GetOperatingSystemMode());
    EXPECT_EQ(MEMC_SOUND_DMA_DISABLE, GetSoundDmaControl());
    EXPECT_EQ(MEMC_VIDEO_DMA_DISABLE, GetVideoDmaControl());
    EXPECT_EQ(MEMC_DRAM_REFRESH_NONE_0, GetDramRefreshControl());
    EXPECT_EQ(MEMC_HIGH_ROM_ACCESS_450, GetHighRomAccessTime());
    EXPECT_EQ(MEMC_LOW_ROM_ACCESS_450, GetLowRomAccessTime());
    EXPECT_EQ(MEMC_PAGE_SIZE_4K, GetPageSize());
}

TEST_F(DmaAddressGeneratorTest, WritingWordsToADmaAddressRegisterSetsTheAppropriateRegister) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x7F0Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = 0x6F1Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = 0x5F2Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0x4F3Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x3F4Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x2F5Fu
    }));
    EXPECT_EQ(0x7F0Fu, GetVideoInit());
    EXPECT_EQ(0x6F1Fu, GetVideoStart());
    EXPECT_EQ(0x5F2Fu, GetVideoEnd());
    EXPECT_EQ(0x4F3Fu, GetCursorInit());
    EXPECT_EQ(0x3F4Fu, GetSoundStart());
    EXPECT_EQ(0x2F5Fu, GetSoundEndNext());
}

TEST_F(DmaAddressGeneratorTest, WritingBytesToADmaAddressRegisterSetsTheAppropriateRegister) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x7F0Fu
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = 0x6F1Fu
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = 0x5F2Fu
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0x4F3Fu
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x3F4Fu
    }));
    EXPECT_TRUE(WriteByteDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x2F5Fu
    }));
    EXPECT_EQ(0x7F0Fu, GetVideoInit());
    EXPECT_EQ(0x6F1Fu, GetVideoStart());
    EXPECT_EQ(0x5F2Fu, GetVideoEnd());
    EXPECT_EQ(0x4F3Fu, GetCursorInit());
    EXPECT_EQ(0x3F4Fu, GetSoundStart());
    EXPECT_EQ(0x2F5Fu, GetSoundEndNext());
}

TEST_F(DmaAddressGeneratorTest, WritingToADmaAddressRegisterInUserModeFails) {
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
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x7F0Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = 0x6F1Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = 0x5F2Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0x4F3Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x3F4Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x2F5Fu
    }));
    EXPECT_EQ(0u, GetVideoInit());
    EXPECT_EQ(0u, GetVideoStart());
    EXPECT_EQ(0u, GetVideoEnd());
    EXPECT_EQ(0u, GetCursorInit());
    EXPECT_EQ(0u, GetSoundStart());
    EXPECT_EQ(0u, GetSoundEndNext());
}

TEST_F(DmaAddressGeneratorTest, WritingToADmaAddressRegisterInOperatingSystemModeFails) {
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
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_INIT,
        .value = 0x7F0Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_START,
        .value = 0x6F1Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_VIDEO_END,
        .value = 0x5F2Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_CURSOR_INIT,
        .value = 0x4F3Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x3F4Fu
    }));
    EXPECT_FALSE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x2F5Fu
    }));
    EXPECT_EQ(0u, GetVideoInit());
    EXPECT_EQ(0u, GetVideoStart());
    EXPECT_EQ(0u, GetVideoEnd());
    EXPECT_EQ(0u, GetCursorInit());
    EXPECT_EQ(0u, GetSoundStart());
    EXPECT_EQ(0u, GetSoundEndNext());
}

TEST_F(DmaAddressGeneratorTest, DefiningTheNextSoundBufferDeassertsTheSoundInterruptLine) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, EndSoundInterrupt);
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x7F0Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x6F1Fu
    }));
    EXPECT_TRUE(GetNextSoundBufferValid());
}

TEST_F(DmaAddressGeneratorTest, WritingToTheSoundPointerRegisterWhenTheNextSoundBufferIsValidSwapsAndAssertsTheSoundInterruptLine) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, EndSoundInterrupt);
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x7F0Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_START,
        .value = 0x6F1Fu
    }));
    EXPECT_TRUE(GetNextSoundBufferValid());
    EXPECT_CALL(mediator, StartSoundInterrupt);
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0u
    }));
    EXPECT_EQ(0x6F1Fu, GetSoundPointer());
    EXPECT_EQ(0x7F0Fu, GetSoundEndCurrent());
    EXPECT_EQ(0u, GetSoundEndNext());
    EXPECT_FALSE(GetNextSoundBufferValid());
}

TEST_F(DmaAddressGeneratorTest, WritingToTheSoundPointerRegisterWhenTheNextSoundBufferIsInvalidResetsAndAssertsTheSoundInterruptLine) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, StartSoundInterrupt);
    EXPECT_FALSE(GetNextSoundBufferValid());
    EXPECT_EQ(0u, GetSoundStart());
    EXPECT_EQ(0u, GetSoundEndCurrent());
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_END_NEXT,
        .value = 0x7F0Fu
    }));
    EXPECT_TRUE(WriteWordDmaAddressGenerator({
        .reg = MEMC_DMA_SOUND_PTR,
        .value = 0u
    }));
    EXPECT_EQ(0u, GetSoundPointer());
    EXPECT_EQ(0u, GetSoundEndCurrent());
    EXPECT_EQ(0x7F0Fu, GetSoundEndNext());
    EXPECT_FALSE(GetNextSoundBufferValid());
}