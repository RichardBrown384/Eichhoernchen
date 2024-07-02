#include "Archimedes/MemcTest.h"

struct ControlWordParam : public ControlWord {
    using TupleT = std::tuple<bool, bool, bool, uint32_t, uint32_t, uint32_t, uint32_t>;

    explicit ControlWordParam(const TupleT& t) : ControlWord() {
        operatingSystemMode = std::get<0>(t);
        soundDmaControl = std::get<1>(t);
        videoDmaControl = std::get<2>(t);
        dramRefreshControl = std::get<3>(t);
        highRomAccessTime = std::get<4>(t);
        lowRomAccessTime = std::get<5>(t);
        logicalPageSize = std::get<6>(t);
    }
};

[[maybe_unused]] std::ostream& operator<<(std::ostream& out, const ControlWordParam& word) {
    out << "[ ";
    out << "osMode = " << std::to_string(word.operatingSystemMode) << ", ";
    out << "soundDma = " << std::to_string(word.soundDmaControl) << ", ";
    out << "videoDma = " << std::to_string(word.videoDmaControl) << ", ";
    out << "dramRefresh = " << std::to_string(word.dramRefreshControl) << ", ";
    out << "highAccess = " << std::to_string(word.highRomAccessTime) << ", ";
    out << "lowAccess = " << std::to_string(word.lowRomAccessTime) << ", ";
    out << "pageSize = " << std::to_string(word.logicalPageSize) << " ";
    out << "]";
    return out;
}

class MemcControlRegisterTest : public MemcTest,
                                public testing::WithParamInterface<ControlWordParam> {
public:
    MemcControlRegisterTest() : MemcTest(0u) {}
    ~MemcControlRegisterTest() override = default;
};

TEST_P(MemcControlRegisterTest, AllWordAccessControlWordPermutationsCorrectlyConfigureTheSystem) {
    const auto& param = GetParam();
    ASSERT_TRUE(WriteWordMemcControl(GetParam()));
    ASSERT_EQ(param.operatingSystemMode, GetOperatingSystemMode());
    ASSERT_EQ(param.soundDmaControl, GetSoundDmaControl());
    ASSERT_EQ(param.videoDmaControl, GetVideoDmaControl());
    ASSERT_EQ(param.dramRefreshControl, GetDramRefreshControl());
    ASSERT_EQ(param.highRomAccessTime, GetHighRomAccessTime());
    ASSERT_EQ(param.lowRomAccessTime, GetLowRomAccessTime());
    ASSERT_EQ(param.logicalPageSize, GetPageSize());
}

TEST_P(MemcControlRegisterTest, AllByteAccessControlWordPermutationsCorrectlyConfigureTheSystem) {
    const auto& param = GetParam();
    ASSERT_TRUE(WriteByteMemcControl(GetParam()));
    ASSERT_EQ(param.operatingSystemMode, GetOperatingSystemMode());
    ASSERT_EQ(param.soundDmaControl, GetSoundDmaControl());
    ASSERT_EQ(param.videoDmaControl, GetVideoDmaControl());
    ASSERT_EQ(param.dramRefreshControl, GetDramRefreshControl());
    ASSERT_EQ(param.highRomAccessTime, GetHighRomAccessTime());
    ASSERT_EQ(param.lowRomAccessTime, GetLowRomAccessTime());
    ASSERT_EQ(param.logicalPageSize, GetPageSize());
}

INSTANTIATE_TEST_SUITE_P(
    ControlWordVariations,
    MemcControlRegisterTest,
    testing::ConvertGenerator<ControlWordParam::TupleT>(testing::Combine(
        testing::Values(MEMC_OS_MODE_DISABLE, MEMC_OS_MODE_ENABLE),
        testing::Values(MEMC_SOUND_DMA_DISABLE, MEMC_SOUND_DMA_ENABLE),
        testing::Values(MEMC_VIDEO_DMA_DISABLE, MEMC_VIDEO_DMA_ENABLE),
        testing::Values(
            MEMC_DRAM_REFRESH_NONE_0,
            MEMC_DRAM_REFRESH_FLY_BACK,
            MEMC_DRAM_REFRESH_NONE_2,
            MEMC_DRAM_REFRESH_CONTINUOUS
        ),
        testing::Values(MEMC_HIGH_ROM_ACCESS_450, MEMC_HIGH_ROM_ACCESS_325,
                        MEMC_HIGH_ROM_ACCESS_250),
        testing::Values(MEMC_LOW_ROM_ACCESS_450, MEMC_LOW_ROM_ACCESS_325,
                        MEMC_LOW_ROM_ACCESS_250),
        testing::Values(MEMC_PAGE_SIZE_4K, MEMC_PAGE_SIZE_8K, MEMC_PAGE_SIZE_16K,
                        MEMC_PAGE_SIZE_32K)
    )));

TEST_F(MemcControlRegisterTest, WritesInUserModeFail) {
    SetSupervisor(false);
    ASSERT_FALSE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_DISABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
}

TEST_F(MemcControlRegisterTest, WritesInOperatingSystemModeFail) {
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
    ASSERT_FALSE(WriteWordMemcControl({
        .operatingSystemMode = MEMC_OS_MODE_ENABLE,
        .soundDmaControl = MEMC_SOUND_DMA_DISABLE,
        .videoDmaControl = MEMC_VIDEO_DMA_DISABLE,
        .dramRefreshControl = MEMC_DRAM_REFRESH_NONE_0,
        .highRomAccessTime = MEMC_HIGH_ROM_ACCESS_450,
        .lowRomAccessTime = MEMC_LOW_ROM_ACCESS_450,
        .logicalPageSize = MEMC_PAGE_SIZE_4K
    }));
}