#include "Archimedes/MemcTest.h"

class PhysicalMemoryTest : public MemcTest {
public:
    PhysicalMemoryTest() : MemcTest(ONE_MEGABYTE) {}
    ~PhysicalMemoryTest() override = default;
};

TEST_F(PhysicalMemoryTest, PhysicalRamIsMirroredSixteenTimes) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE;
        uint32_t v{};
        EXPECT_TRUE(WriteWordPhysical(address, 0xBADC'0FFEu));
        EXPECT_TRUE(ReadWordPhysical(address, v));
        EXPECT_TRUE(WriteWordPhysical(address, 0u));
        EXPECT_EQ(0xBADC'0FFEu, v);
    }
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE + 512u;
        uint32_t v{};
        EXPECT_TRUE(WriteBytePhysical(address, 0xDFu));
        EXPECT_TRUE(ReadBytePhysical(address, v));
        EXPECT_TRUE(WriteBytePhysical(address, 0u));
        EXPECT_EQ(0xDFu, v);
    }
}

TEST_F(PhysicalMemoryTest, PhysicalRamMirrorsDenyAccessInUserMode) {
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
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE;
        uint32_t v{};
        EXPECT_FALSE(WriteWordPhysical(address, 0xBADC'0FFEu));
        EXPECT_FALSE(ReadWordPhysical(address, v));
        EXPECT_FALSE(WriteWordPhysical(address, 0u));
        EXPECT_EQ(0u, v);
    }
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE + 512u;
        uint32_t v{};
        EXPECT_FALSE(WriteBytePhysical(address, 0xDFu));
        EXPECT_FALSE(ReadBytePhysical(address, v));
        EXPECT_FALSE(WriteBytePhysical(address, 0u));
        EXPECT_EQ(0u, v);
    }
}

TEST_F(PhysicalMemoryTest, PhysicalRamMirrorsDenyAccessInOperatingSystemMode) {
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
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE;
        uint32_t v{};
        EXPECT_FALSE(WriteWordPhysical(address, 0xBADC'0FFEu));
        EXPECT_FALSE(ReadWordPhysical(address, v));
        EXPECT_FALSE(WriteWordPhysical(address, 0u));
        EXPECT_EQ(0u, v);
    }
    for (auto i = 0u; i < 16u; ++i) {
        const auto address = i * ONE_MEGABYTE + 512u;
        uint32_t v{};
        EXPECT_FALSE(WriteBytePhysical(address, 0xDFu));
        EXPECT_FALSE(ReadBytePhysical(address, v));
        EXPECT_FALSE(WriteBytePhysical(address, 0u));
        EXPECT_EQ(0u, v);
    }
}
