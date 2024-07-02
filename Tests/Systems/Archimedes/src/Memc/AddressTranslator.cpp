#include "Archimedes/MemcTest.h"

class AddressTranslatorTest: public MemcTest {
public:
    AddressTranslatorTest() : MemcTest(FOUR_MEGABYTES) {}
    ~AddressTranslatorTest() override = default;
};

TEST_F(AddressTranslatorTest, WhenTheAddressTranslatorHasNotBeenProgrammedReadingAndWritingToAnyLogicalPageShouldFail) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    uint32_t discard;
    EXPECT_FALSE(WriteWordLogical(2u, 0u, 2u));
    EXPECT_FALSE(ReadWordLogical(2u, 0u, discard));
}

TEST_F(AddressTranslatorTest, WhenTheAddressTranslatorIsWrittenToInOperatingSystemModeItShouldFail) {
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
    ASSERT_FALSE(WriteWordAddressTranslator({
        .physicalPageNumber = 1u,
        .logicalPageNumber = 3u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
}

TEST_F(AddressTranslatorTest, WhenTheAddressTranslatorIsWrittenToInUserModeItShouldFail) {
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
    ASSERT_FALSE(WriteWordAddressTranslator({
        .physicalPageNumber = 8u,
        .logicalPageNumber = 9u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
}

TEST_F(AddressTranslatorTest, WhenA4kLogicalPageIsMappedToAPhysicalPageTheDataIsMirrored) {
    constexpr auto logicalPage = 2u;
    constexpr auto physicalPage = 1u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPage,
        .logicalPageNumber = logicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    uint32_t v;
    // Write word to the start of the 4k logical page
    EXPECT_TRUE(WriteWordLogical(logicalPage, 0u, 0xBBCC'AAEEu));
    EXPECT_TRUE(ReadWordPhysical(physicalPage, 0u, v));
    EXPECT_EQ(0xBBCC'AAEEu, v);
    // Write word to the middle of a 4k logical page
    EXPECT_TRUE(WriteWordLogical(logicalPage, 2044u, 0xCCBB'EEAAu));
    EXPECT_TRUE(ReadWordPhysical(physicalPage, 2044u, v));
    EXPECT_EQ(0xCCBB'EEAAu, v);
    // Write word to the end of a 4k logical page
    EXPECT_TRUE(WriteWordLogical(logicalPage, 4092u, 0xFF11'3535u));
    EXPECT_TRUE(ReadWordPhysical(physicalPage, 4092u, v));
    EXPECT_EQ(0xFF11'3535u, v);
    // Write word to the start of the physical page
    EXPECT_TRUE(WriteWordPhysical(physicalPage, 0u, 0xABAB'ABABu));
    EXPECT_TRUE(ReadWordLogical(logicalPage, 0u, v));
    EXPECT_EQ(0xABAB'ABABu, v);
}

TEST_F(AddressTranslatorTest, WhenTheAddressTranslatorIsProgrammedWithByteAccessTheLogicalPageIsMapped) {
    constexpr auto logicalPage = 2u;
    constexpr auto physicalPage = 1u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteByteAddressTranslator({
        .physicalPageNumber = physicalPage,
        .logicalPageNumber = logicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    uint32_t v;
    // Write word to the start of the 4k logical page
    EXPECT_TRUE(WriteWordLogical(logicalPage, 0u, 0xBBCC'AAEEu));
    EXPECT_TRUE(ReadWordPhysical(physicalPage, 0u, v));
    EXPECT_EQ(0xBBCC'AAEEu, v);
}

TEST_F(AddressTranslatorTest, WhenA4kLogicalPageIsRemovedFromTheAddressTranslatorSubsequentInteractionsFail) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    // Map logical page 3 to physical page 18 and write data to it
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 18u,
        .logicalPageNumber = 3u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordLogical(3u, 12u, 0xCAFE'CAFE));
    // Map logical page 4 to physical page 18 (thereby removing logical page 3)
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 18u,
        .logicalPageNumber = 4u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    EXPECT_FALSE(WriteWordLogical(3u, 0u, 0xBBCC'AAEEu));
    EXPECT_FALSE(WriteWordLogical(3u, 2044u, 0xBBCC'AAEEu));
    EXPECT_FALSE(WriteWordLogical(3u, 4092u, 0xBBCC'AAEEu));
}