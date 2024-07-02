#include "Archimedes/MemcTest.h"

class LogicalMemoryTestHalfMegabyte : public MemcTest {
public:
    LogicalMemoryTestHalfMegabyte() : MemcTest(HALF_MEGABYTE) {}
    ~LogicalMemoryTestHalfMegabyte() override = default;
    void SetUp() override {
        MemcTest::SetUp();
        ASSERT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
            .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
            .logicalPageSize = MEMC_PAGE_SIZE_4K
        }));
    }
};

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel3AllOperationsFailInUserMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_3
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_FALSE(ReadWordLogical(logicalPageNumber, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_FALSE(ReadByteLogical(logicalPageNumber, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel2AllOperationsFailInUserMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_2
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_FALSE(ReadWordLogical(logicalPageNumber, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_FALSE(ReadByteLogical(logicalPageNumber, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel1WritesFailInUserMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_1
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel0AllOperationSucceedInUserMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel3WritesFailInOperatingSystemMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_3
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel2WritesFailInOperatingSystemMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_2
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel1AllOperationSucceedInOperatingSystemMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_1
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel0AllOperationSucceedInOperatingSystemMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel3AllOperationSucceedInSupervisorMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_3
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel2AllOperationSucceedInSupervisorMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_2
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel1AllOperationSucceedInSupervisorMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_1
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, WhenALogicalPageIsMappedWithPageProtectionLevel0AllOperationSucceedInSupervisorMode) {
    const auto physicalPageNumber = 125u;
    const auto logicalPageNumber = 400u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = physicalPageNumber,
        .logicalPageNumber = logicalPageNumber,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(logicalPageNumber, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(logicalPageNumber, 200u, v));
    EXPECT_EQ(0xBBCC'1133u, v);
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(logicalPageNumber, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 400u, v));
    EXPECT_EQ(0x22u, v);
    // Validate byte read from address we wrote to with a word
    EXPECT_TRUE(ReadByteLogical(logicalPageNumber, 203u, v));
    EXPECT_EQ(0xBBu, v);
}

TEST_F(LogicalMemoryTestHalfMegabyte, ContentedLogicalPagesRemainMappedWhenThePhysicalPageIsReassigned) {
    constexpr auto contendedLogicalPage = 8u;
    constexpr auto contendedPhysicalPage = 3u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 4,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = 9,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(contendedLogicalPage, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(contendedLogicalPage, 200u, v));
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(contendedLogicalPage, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(contendedLogicalPage, 400u, v));
}

TEST_F(LogicalMemoryTestHalfMegabyte, ContendedLogicalPagesRemainMappedIrrespectiveOfTheOrderInWhichTheyWhereProgrammed) {
    constexpr auto contendedLogicalPage = 8u;
    constexpr auto contendedPhysicalPage = 3u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 4,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = 9,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    uint32_t v {};
    // Word accesses
    EXPECT_TRUE(WriteWordLogical(contendedLogicalPage, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(contendedLogicalPage, 200u, v));
    // Byte accesses
    EXPECT_TRUE(WriteByteLogical(contendedLogicalPage, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(contendedLogicalPage, 400u, v));
}


TEST_F(LogicalMemoryTestHalfMegabyte, ContendedLogicalPagesHaveTheCorrectPageProtectionLevelsAssigned) {
    constexpr auto contendedLogicalPage = 8u;
    constexpr auto contendedPhysicalPage = 3u;
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 4,
        .logicalPageNumber = contendedLogicalPage,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_1
    }));
    ASSERT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = contendedPhysicalPage,
        .logicalPageNumber = 9,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    SetSupervisor(false);
    uint32_t v {};
    // Word accesses
    EXPECT_FALSE(WriteWordLogical(contendedLogicalPage, 200u, 0xBBCC'1133u));
    EXPECT_TRUE(ReadWordLogical(contendedLogicalPage, 200u, v));
    // Byte accesses
    EXPECT_FALSE(WriteByteLogical(contendedLogicalPage, 400u, 0x22u));
    EXPECT_TRUE(ReadByteLogical(contendedLogicalPage, 400u, v));
}