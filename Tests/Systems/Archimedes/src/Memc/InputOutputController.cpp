#include "Archimedes/MemcTest.h"

class InputOutputControllerTest: public MemcTest {
public:
    InputOutputControllerTest() : MemcTest(0u) {}
    ~InputOutputControllerTest() override = default;
};

TEST_F(InputOutputControllerTest, InputOutputControllerIsAccessibleInSupervisorMode) {
    ASSERT_TRUE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, 0xFFCCEEDDu));
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, 0xCCCCCCCCu));
    EXPECT_CALL(mediator, ReadWordIoc(testing::_))
        .WillOnce(testing::Return(0xAABBCC11u))
        .WillOnce(testing::Return(0x23232323u));
    EXPECT_TRUE(WriteWordInputOutputController(0x0u, 0xFFCCEEDDu));
    EXPECT_TRUE(WriteByteInputOutputController(0x0u, 0xCCu));
    uint32_t byte, word;
    EXPECT_TRUE(ReadWordInputOutputController(0u, word));
    EXPECT_TRUE(ReadByteInputOutputController(0u, byte));
    EXPECT_EQ(0xAABBCC11u, word);
    EXPECT_EQ(0x23u, byte);
}

TEST_F(InputOutputControllerTest, InputOutputControllerIsNotAccessibleInOperatingSystemMode) {
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
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, ReadWordIoc(testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, ReadWordIoc(testing::_))
        .Times(0u);
    EXPECT_FALSE(WriteWordInputOutputController(0x0u, 0xFFCCEEDDu));
    EXPECT_FALSE(WriteByteInputOutputController(0x0u, 0xCCu));
    uint32_t discard;
    EXPECT_FALSE(ReadWordInputOutputController(0u, discard));
    EXPECT_FALSE(ReadByteInputOutputController(0u, discard));
}

TEST_F(InputOutputControllerTest, InputOutputControllerIsNotAccessibleInUserMode) {
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
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, WriteWordIoc(testing::_, testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, ReadWordIoc(testing::_))
        .Times(0u);
    EXPECT_CALL(mediator, ReadWordIoc(testing::_))
        .Times(0u);
    EXPECT_FALSE(WriteWordInputOutputController(0x0u, 0xFFCCEEDDu));
    EXPECT_FALSE(WriteByteInputOutputController(0x0u, 0xCCu));
    uint32_t discard;
    EXPECT_FALSE(ReadWordInputOutputController(0u, discard));
    EXPECT_FALSE(ReadByteInputOutputController(0u, discard));
}