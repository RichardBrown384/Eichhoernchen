#include "Archimedes/MemcTest.h"

class LowRomTest: public MemcTest {
public:
    LowRomTest() : MemcTest(0u) {}
    ~LowRomTest() override = default;
};

TEST_F(LowRomTest, LowRomIsAccessibleInSupervisorMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    uint32_t x, y, z;
    EXPECT_TRUE(ReadWordLowRom(0u, x));
    EXPECT_TRUE(ReadWordLowRom(4u, y));
    EXPECT_TRUE(ReadWordLowRom(8u, z));
    EXPECT_EQ(0x04030201u, x);
    EXPECT_EQ(0x08070605u, y);
    EXPECT_EQ(0x0C0B0A09u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteLowRom(0u, a));
    EXPECT_TRUE(ReadByteLowRom(5u, b));
    EXPECT_TRUE(ReadByteLowRom(10u, c));
    EXPECT_EQ(0x01u, a);
    EXPECT_EQ(0x06u, b);
    EXPECT_EQ(0x0Bu, c);
}

TEST_F(LowRomTest, LowRomIsAccessibleInOperatingSystemMode) {
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
    uint32_t x, y, z;
    EXPECT_TRUE(ReadWordLowRom(0u, x));
    EXPECT_TRUE(ReadWordLowRom(4u, y));
    EXPECT_TRUE(ReadWordLowRom(8u, z));
    EXPECT_EQ(0x04030201u, x);
    EXPECT_EQ(0x08070605u, y);
    EXPECT_EQ(0x0C0B0A09u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteLowRom(0u, a));
    EXPECT_TRUE(ReadByteLowRom(5u, b));
    EXPECT_TRUE(ReadByteLowRom(10u, c));
    EXPECT_EQ(0x01u, a);
    EXPECT_EQ(0x06u, b);
    EXPECT_EQ(0x0Bu, c);
}

TEST_F(LowRomTest, LowRomIsAccessibleInUserModeMode) {
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
    uint32_t x, y, z;
    EXPECT_TRUE(ReadWordLowRom(0u, x));
    EXPECT_TRUE(ReadWordLowRom(4u, y));
    EXPECT_TRUE(ReadWordLowRom(8u, z));
    EXPECT_EQ(0x04030201u, x);
    EXPECT_EQ(0x08070605u, y);
    EXPECT_EQ(0x0C0B0A09u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteLowRom(0u, a));
    EXPECT_TRUE(ReadByteLowRom(5u, b));
    EXPECT_TRUE(ReadByteLowRom(10u, c));
    EXPECT_EQ(0x01u, a);
    EXPECT_EQ(0x06u, b);
    EXPECT_EQ(0x0Bu, c);
}