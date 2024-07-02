#include "Archimedes/MemcTest.h"

class AccessTimingTest: public MemcTest {
public:
    AccessTimingTest() : MemcTest(ONE_MEGABYTE) {}
    ~AccessTimingTest() override = default;
};

TEST_F(AccessTimingTest, LogicalRamNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_ADDRESS_TRANSLATOR_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS))
            .Times(16);
    }
    CycleN();
    uint32_t discard;
    EXPECT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 0u,
        .logicalPageNumber = 0u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordLogical(0u, address, discard));
        EXPECT_TRUE(ReadByteLogical(0u, address, discard));
        EXPECT_TRUE(WriteWordLogical(0u, address, 0u));
        EXPECT_TRUE(WriteByteLogical(0u, address, 0u));
    }
}

TEST_F(AccessTimingTest, LogicalRamSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_ADDRESS_TRANSLATOR_NS));
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS))
            .Times(4);
        EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS))
            .Times(12);
    }
    CycleS();
    uint32_t discard;
    EXPECT_TRUE(WriteWordAddressTranslator({
        .physicalPageNumber = 0u,
        .logicalPageNumber = 0u,
        .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
    }));
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordLogical(0u, address, discard));
        EXPECT_TRUE(ReadByteLogical(0u, address, discard));
        EXPECT_TRUE(WriteWordLogical(0u, address, 0u));
        EXPECT_TRUE(WriteByteLogical(0u, address, 0u));
    }
}

TEST_F(AccessTimingTest, PhysicalRamNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS))
            .Times(16);
    }
    CycleN();
    uint32_t discard;
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordPhysical(address, discard));
        EXPECT_TRUE(ReadBytePhysical(address, discard));
        EXPECT_TRUE(WriteWordPhysical(address, 0u));
        EXPECT_TRUE(WriteBytePhysical(address, 0u));
    }
}

TEST_F(AccessTimingTest, PhysicalRamSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_N_CYCLE_NS))
            .Times(4);
        EXPECT_CALL(mediator, Update(MEMC_RAM_S_CYCLE_NS))
            .Times(12);
    }
    CycleS();
    uint32_t discard;
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordPhysical(address, discard));
        EXPECT_TRUE(ReadBytePhysical(address, discard));
        EXPECT_TRUE(WriteWordPhysical(address, 0u));
        EXPECT_TRUE(WriteBytePhysical(address, 0u));
    }
}

TEST_F(AccessTimingTest, IoControllerNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_IO_CYCLE_NS))
            .Times(16);
    }
    CycleN();
    uint32_t discard;
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordInputOutputController(address, discard));
        EXPECT_TRUE(ReadByteInputOutputController(address, discard));
        EXPECT_TRUE(WriteWordInputOutputController(address, 0u));
        EXPECT_TRUE(WriteByteInputOutputController(address, 0u));
    }
}

TEST_F(AccessTimingTest, IoControllerSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_IO_CYCLE_NS))
            .Times(16);
    }
    CycleS();
    uint32_t discard;
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(ReadWordInputOutputController(address, discard));
        EXPECT_TRUE(ReadByteInputOutputController(address, discard));
        EXPECT_TRUE(WriteWordInputOutputController(address, 0u));
        EXPECT_TRUE(WriteByteInputOutputController(address, 0u));
    }
}

TEST_F(AccessTimingTest, LowRomNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        for (const auto speed: {
            MEMC_ROM_CYCLE_450_NS,
            MEMC_ROM_CYCLE_325_NS,
            MEMC_ROM_CYCLE_200_NS }) {
            EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
            EXPECT_CALL(mediator, Update(speed))
                .Times(8);
        }
    }
    CycleN();
    uint32_t discard;
    for (const auto speed: { MEMC_LOW_ROM_ACCESS_450, MEMC_LOW_ROM_ACCESS_325, MEMC_LOW_ROM_ACCESS_250 }) {
        EXPECT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
            .lowRomAccessTime = speed,
            .logicalPageSize = MEMC_PAGE_SIZE_4K
        }));
        for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
            EXPECT_TRUE(ReadWordLowRom(address, discard));
            EXPECT_TRUE(ReadByteLowRom(address, discard));
        }
    }
}

TEST_F(AccessTimingTest, LowRomSequentialAccessTiming) {
    {
        testing::InSequence s;
        for (const auto speed: {
            MEMC_ROM_CYCLE_450_NS,
            MEMC_ROM_CYCLE_325_NS,
            MEMC_ROM_CYCLE_200_NS }) {
            EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
            EXPECT_CALL(mediator, Update(speed))
                .Times(8);
        }
    }
    CycleS();
    uint32_t discard;
    for (const auto speed: { MEMC_LOW_ROM_ACCESS_450, MEMC_LOW_ROM_ACCESS_325, MEMC_LOW_ROM_ACCESS_250 }) {
        ASSERT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
            .lowRomAccessTime = speed,
            .logicalPageSize = MEMC_PAGE_SIZE_4K
        }));
        for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
            EXPECT_TRUE(ReadWordLowRom(address, discard));
            EXPECT_TRUE(ReadByteLowRom(address, discard));
        }
    }
}

TEST_F(AccessTimingTest, HighRomNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        for (const auto speed: {
            MEMC_ROM_CYCLE_450_NS,
            MEMC_ROM_CYCLE_325_NS,
            MEMC_ROM_CYCLE_200_NS }) {
            EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
            EXPECT_CALL(mediator, Update(speed))
                .Times(8);
        }
    }
    CycleN();
    uint32_t discard;
    for (const auto speed: { MEMC_LOW_ROM_ACCESS_450, MEMC_LOW_ROM_ACCESS_325, MEMC_LOW_ROM_ACCESS_250 }) {
        EXPECT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = speed,
            .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
            .logicalPageSize = MEMC_PAGE_SIZE_4K
        }));
        for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
            EXPECT_TRUE(ReadWordHighRom(address, discard));
            EXPECT_TRUE(ReadByteHighRom(address, discard));
        }
    }
}

TEST_F(AccessTimingTest, HighRomSequentialAccessTiming) {
    {
        testing::InSequence s;
        for (const auto speed: {
            MEMC_ROM_CYCLE_450_NS,
            MEMC_ROM_CYCLE_325_NS,
            MEMC_ROM_CYCLE_200_NS }) {
            EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS));
            EXPECT_CALL(mediator, Update(speed))
                .Times(8);
        }
    }
    CycleS();
    uint32_t discard;
    for (const auto speed: { MEMC_LOW_ROM_ACCESS_450, MEMC_LOW_ROM_ACCESS_325, MEMC_LOW_ROM_ACCESS_250 }) {
        EXPECT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = speed,
            .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
            .logicalPageSize = MEMC_PAGE_SIZE_4K
        }));
        for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
            EXPECT_TRUE(ReadWordHighRom(address, discard));
            EXPECT_TRUE(ReadByteHighRom(address, discard));
        }
    }
}

TEST_F(AccessTimingTest, VideoControllerNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_VIDC_CYCLE_NS))
            .Times(4);
    }
    CycleN();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(WriteWordVideoController(address, 0u));
    }
}

TEST_F(AccessTimingTest, VideoControllerSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_VIDC_CYCLE_NS))
            .Times(4);
    }
    CycleS();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(WriteWordVideoController(address, 0u));
    }
}

TEST_F(AccessTimingTest, DmaControlNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(28);
    }
    CycleN();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        for (const auto reg: {
            MEMC_DMA_VIDEO_INIT,
            MEMC_DMA_VIDEO_START,
            MEMC_DMA_VIDEO_END,
            MEMC_DMA_CURSOR_INIT,
            MEMC_DMA_SOUND_START,
            MEMC_DMA_SOUND_END_NEXT,
            MEMC_DMA_SOUND_PTR }) {
            EXPECT_TRUE(WriteWordDmaAddressGenerator({
                .reg = reg,
                .value = address
            }));
        }
    }
}

TEST_F(AccessTimingTest, DmaControlSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_DMA_CONTROL_CYCLE_NS))
            .Times(28);
    }
    CycleS();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        for (const auto reg: {
            MEMC_DMA_VIDEO_INIT,
            MEMC_DMA_VIDEO_START,
            MEMC_DMA_VIDEO_END,
            MEMC_DMA_CURSOR_INIT,
            MEMC_DMA_SOUND_START,
            MEMC_DMA_SOUND_END_NEXT,
            MEMC_DMA_SOUND_PTR }) {
            EXPECT_TRUE(WriteWordDmaAddressGenerator({
                .reg = reg,
                .value = address
            }));
        }
    }
}

TEST_F(AccessTimingTest, MemcControllerNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS))
            .Times(4);
    }
    CycleN();
    for (const auto pageSize: { // NB page size is encoded in address lines 2 and 3
        MEMC_PAGE_SIZE_4K,
        MEMC_PAGE_SIZE_8K,
        MEMC_PAGE_SIZE_16K,
        MEMC_PAGE_SIZE_32K }) {
        EXPECT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
            .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
            .logicalPageSize = pageSize
        }));
    }
}

TEST_F(AccessTimingTest, MemcControllerSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_MEMC_CONTROL_CYCLE_NS))
            .Times(4);
    }
    CycleS();
    for (const auto pageSize: { // NB page size is encoded in address lines 2 and 3
        MEMC_PAGE_SIZE_4K,
        MEMC_PAGE_SIZE_8K,
        MEMC_PAGE_SIZE_16K,
        MEMC_PAGE_SIZE_32K }) {
        EXPECT_TRUE(WriteWordMemcControl({
            .operatingSystemMode = MEMC_OS_MODE_DISABLE,
            .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
            .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
            .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
            .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
            .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
            .logicalPageSize = pageSize
        }));
    }
}

TEST_F(AccessTimingTest, AddressTranslatorNonSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_ADDRESS_TRANSLATOR_NS))
            .Times(4);
    }
    CycleN();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(WriteWordAddressTranslator({
            .physicalPageNumber = address, // Physical page number is mapped to A6-A0
            .logicalPageNumber = 0u,
            .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
        }));
    }
}

TEST_F(AccessTimingTest, AddressTranslatorSequentialAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_ADDRESS_TRANSLATOR_NS))
            .Times(4);
    }
    CycleS();
    for (const auto address: { 0x0u, 0x4u, 0x8u, 0xCu }) {
        EXPECT_TRUE(WriteWordAddressTranslator({
            .physicalPageNumber = address, // Physical page number is mapped to A6-A0
            .logicalPageNumber = 0u,
            .pageProtectionLevel = MEMC_PAGE_PROTECTION_LEVEL_0
        }));
    }
}

TEST_F(AccessTimingTest, InternalCycleAccessTiming) {
    {
        testing::InSequence s;
        EXPECT_CALL(mediator, Update(MEMC_RAM_I_CYCLE_NS))
            .Times(7);
    }
    CycleI(7);
}