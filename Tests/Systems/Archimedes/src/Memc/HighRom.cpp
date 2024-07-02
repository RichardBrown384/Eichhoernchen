#include "Archimedes/MemcTest.h"

class HighRomTest: public MemcTest {
public:
    HighRomTest() : MemcTest(ONE_MEGABYTE) {}
    ~HighRomTest() override = default;
};

TEST_F(HighRomTest, HighRomIsAccessibleInSupervisorMode) {
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
    EXPECT_TRUE(ReadWordHighRom(0u, x));
    EXPECT_TRUE(ReadWordHighRom(4u, y));
    EXPECT_TRUE(ReadWordHighRom(8u, z));
    EXPECT_EQ(0x14131211u, x);
    EXPECT_EQ(0x18171615u, y);
    EXPECT_EQ(0x1C1B1A19u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteHighRom(0u, a));
    EXPECT_TRUE(ReadByteHighRom(5u, b));
    EXPECT_TRUE(ReadByteHighRom(10u, c));
    EXPECT_EQ(0x11u, a);
    EXPECT_EQ(0x16u, b);
    EXPECT_EQ(0x1Bu, c);
}

TEST_F(HighRomTest, HighRomIsAccessibleInOperatingSystemMode) {
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
    EXPECT_TRUE(ReadWordHighRom(0u, x));
    EXPECT_TRUE(ReadWordHighRom(4u, y));
    EXPECT_TRUE(ReadWordHighRom(8u, z));
    EXPECT_EQ(0x14131211u, x);
    EXPECT_EQ(0x18171615u, y);
    EXPECT_EQ(0x1C1B1A19u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteHighRom(0u, a));
    EXPECT_TRUE(ReadByteHighRom(5u, b));
    EXPECT_TRUE(ReadByteHighRom(10u, c));
    EXPECT_EQ(0x11u, a);
    EXPECT_EQ(0x16u, b);
    EXPECT_EQ(0x1Bu, c);
}

TEST_F(HighRomTest, HighRomIsAccessibleInUserModeMode) {
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
    EXPECT_TRUE(ReadWordHighRom(0u, x));
    EXPECT_TRUE(ReadWordHighRom(4u, y));
    EXPECT_TRUE(ReadWordHighRom(8u, z));
    EXPECT_EQ(0x14131211u, x);
    EXPECT_EQ(0x18171615u, y);
    EXPECT_EQ(0x1C1B1A19u, z);
    uint32_t a, b, c;
    EXPECT_TRUE(ReadByteHighRom(0u, a));
    EXPECT_TRUE(ReadByteHighRom(5u, b));
    EXPECT_TRUE(ReadByteHighRom(10u, c));
    EXPECT_EQ(0x11u, a);
    EXPECT_EQ(0x16u, b);
    EXPECT_EQ(0x1Bu, c);
}

TEST_F(HighRomTest, WhenRomIsContinuouslyEnabledRomIsReadFromLogicalRam) {
    SetRomContinuouslyEnabled(true);
    uint32_t x, y, z;
    EXPECT_TRUE(ReadWordLogical(0u, x));
    EXPECT_TRUE(ReadWordLogical(4u, y));
    EXPECT_TRUE(ReadWordLogical(8u, z));
    EXPECT_EQ(0x14131211u, x);
    EXPECT_EQ(0x18171615u, y);
    EXPECT_EQ(0x1C1B1A19u, z);
}

TEST_F(HighRomTest, WhenRomIsContinuouslyEnabledReadingFromAddressWithBit25SetRestoresStandardOperation) {
    SetRomContinuouslyEnabled(true);
    uint32_t x;
    EXPECT_TRUE(ReadWordLowRom(0u, x));
    EXPECT_EQ(0x04030201u, x);
    EXPECT_FALSE(GetRomContinuouslyEnabled());
}