#include "Archimedes/ArchimedesMemc.h"
#include "Archimedes/ArchimedesMock.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

constexpr auto HALF_MEGABYTE = 512u * 1024u;
constexpr auto ONE_MEGABYTE = 1u * 1024u * 1024u;
constexpr auto TWO_MEGABYTES = 2u * 1024u * 1024u;
constexpr auto FOUR_MEGABYTES = 4u * 1024u * 1024u;

constexpr auto MEMC_OS_MODE_DISABLE = false;
constexpr auto MEMC_OS_MODE_ENABLE = true;

constexpr auto MEMC_SOUND_DMA_DISABLE = false;
constexpr auto MEMC_SOUND_DMA_ENABLE = true;

constexpr auto MEMC_VIDEO_DMA_DISABLE = false;
constexpr auto MEMC_VIDEO_DMA_ENABLE = true;

constexpr auto MEMC_DRAM_REFRESH_NONE_0 = 0u;
constexpr auto MEMC_DRAM_REFRESH_FLY_BACK = 1u;
constexpr auto MEMC_DRAM_REFRESH_NONE_2 = 2u;
constexpr auto MEMC_DRAM_REFRESH_CONTINUOUS = 3u;

constexpr auto MEMC_HIGH_ROM_ACCESS_450 = 0u;
constexpr auto MEMC_HIGH_ROM_ACCESS_325 = 1u;
constexpr auto MEMC_HIGH_ROM_ACCESS_250 = 2u;

constexpr auto MEMC_LOW_ROM_ACCESS_450 = 0u;
constexpr auto MEMC_LOW_ROM_ACCESS_325 = 1u;
constexpr auto MEMC_LOW_ROM_ACCESS_250 = 2u;

constexpr auto MEMC_PAGE_SIZE_4K = 0u;
constexpr auto MEMC_PAGE_SIZE_8K = 1u;
constexpr auto MEMC_PAGE_SIZE_16K = 2u;
constexpr auto MEMC_PAGE_SIZE_32K = 3u;

constexpr auto MEMC_PAGE_PROTECTION_LEVEL_0 = 0u;
constexpr auto MEMC_PAGE_PROTECTION_LEVEL_1 = 1u;
constexpr auto MEMC_PAGE_PROTECTION_LEVEL_2 = 2u;
constexpr auto MEMC_PAGE_PROTECTION_LEVEL_3 = 3u;

constexpr auto MEMC_DMA_VIDEO_INIT = 0u;
constexpr auto MEMC_DMA_VIDEO_START = 1u;
constexpr auto MEMC_DMA_VIDEO_END = 2u;
constexpr auto MEMC_DMA_CURSOR_INIT = 3u;
constexpr auto MEMC_DMA_SOUND_START = 4u;
constexpr auto MEMC_DMA_SOUND_END_NEXT = 5u;
constexpr auto MEMC_DMA_SOUND_PTR = 6u;

constexpr auto MEMC_RAM_N_CYCLE_NS = 250u;
constexpr auto MEMC_RAM_S_CYCLE_NS = 125u;
constexpr auto MEMC_RAM_I_CYCLE_NS = 125u;

constexpr auto MEMC_IO_CYCLE_NS = 250u;

constexpr auto MEMC_ROM_CYCLE_450_NS = 450u;
constexpr auto MEMC_ROM_CYCLE_325_NS = 325u;
constexpr auto MEMC_ROM_CYCLE_200_NS = 200u;

constexpr auto MEMC_VIDC_CYCLE_NS = 125u;

constexpr auto MEMC_DMA_CONTROL_CYCLE_NS = 125u;
constexpr auto MEMC_MEMC_CONTROL_CYCLE_NS = 125u;

constexpr auto MEMC_ADDRESS_TRANSLATOR_NS = 125u;

// Address translation to address line mapping

struct AddressTranslation {
    uint32_t physicalPageNumber;
    uint32_t logicalPageNumber;
    uint32_t pageProtectionLevel;
};

// Control word to address line mapping

struct ControlWord {
    bool operatingSystemMode;
    bool soundDmaControl;
    bool videoDmaControl;
    uint32_t dramRefreshControl;
    uint32_t highRomAccessTime;
    uint32_t lowRomAccessTime;
    uint32_t logicalPageSize;
};

// Dma address generators to address line mapping
struct DmaAddressGeneratorWord {
    uint32_t reg;
    uint32_t value;
};

class MemcTest : public testing::Test {
public:
    explicit MemcTest(uint32_t);
    ~MemcTest() override = default;

    void SetUp() override;

protected:
    auto Reset() -> void;
    auto EndFlyBack() -> void;
    auto StartVideoDmaRequest() -> void;
    auto StartCursorDmaRequest() -> void;
    auto StartSoundDmaRequest() -> void;
    auto EndVideoDmaRequest() -> void;
    auto EndCursorDmaRequest() -> void;
    auto EndSoundDmaRequest() -> void;

    auto SetSupervisor(bool) -> void;
    auto SetRomContinuouslyEnabled(bool) -> void;

    auto CycleS() -> void;
    auto CycleN() -> void;
    auto CycleI(uint32_t) -> void;

    [[nodiscard]] auto GetOperatingSystemMode() const -> bool;
    [[nodiscard]] auto GetSoundDmaControl() const -> bool;
    [[nodiscard]] auto GetVideoDmaControl() const -> bool;
    [[nodiscard]] auto GetDramRefreshControl() const -> uint32_t;
    [[nodiscard]] auto GetHighRomAccessTime() const -> uint32_t;
    [[nodiscard]] auto GetLowRomAccessTime() const -> uint32_t;
    [[nodiscard]] auto GetPageSize() const -> uint32_t;
    [[nodiscard]] auto GetVideoInit() const -> uint32_t;
    [[nodiscard]] auto GetVideoStart() const -> uint32_t;
    [[nodiscard]] auto GetVideoEnd() const -> uint32_t;
    [[nodiscard]] auto GetCursorInit() const -> uint32_t;
    [[nodiscard]] auto GetSoundStart() const -> uint32_t;
    [[nodiscard]] auto GetSoundEndNext() const -> uint32_t;
    [[nodiscard]] auto GetSoundEndCurrent() const -> uint32_t;
    [[nodiscard]] auto GetNextSoundBufferValid() const-> bool;

    [[nodiscard]] auto GetVideoPointer() const -> uint32_t;
    [[nodiscard]] auto GetCursorPointer() const -> uint32_t;
    [[nodiscard]] auto GetSoundPointer() const -> uint32_t;

    [[nodiscard]] auto GetVideoDmaRequests() const -> uint32_t;
    [[nodiscard]] auto GetCursorDmaRequests() const -> uint32_t;
    [[nodiscard]] auto GetSoundDmaRequests() const -> uint32_t;

    [[nodiscard]] auto GetSupervisor() const -> bool;
    [[nodiscard]] auto GetRomContinuouslyEnabled() const -> bool;

    auto ReadWordLogical(uint32_t, uint32_t&) -> bool;
    auto ReadWordLogical(uint32_t, uint32_t, uint32_t&) -> bool;
    auto ReadWordPhysical(uint32_t, uint32_t&) -> bool;
    auto ReadWordPhysical(uint32_t, uint32_t, uint32_t&) -> bool;
    auto ReadWordInputOutputController(uint32_t, uint32_t&) -> bool;
    auto ReadWordLowRom(uint32_t, uint32_t&) -> bool;
    auto ReadWordHighRom(uint32_t, uint32_t&) -> bool;

    auto ReadByteLogical(uint32_t, uint32_t&) -> bool;
    auto ReadByteLogical(uint32_t, uint32_t, uint32_t&) -> bool;
    auto ReadBytePhysical(uint32_t, uint32_t&) -> bool;
    auto ReadByteInputOutputController(uint32_t, uint32_t&) -> bool;
    auto ReadByteLowRom(uint32_t, uint32_t&) -> bool;
    auto ReadByteHighRom(uint32_t, uint32_t&) -> bool;

    auto WriteWordLogical(uint32_t, uint32_t) -> bool;
    auto WriteWordLogical(uint32_t, uint32_t, uint32_t) -> bool;
    auto WriteWordPhysical(uint32_t, uint32_t) -> bool;
    auto WriteWordPhysical(uint32_t, uint32_t, uint32_t) -> bool;
    auto WriteWordInputOutputController(uint32_t, uint32_t) -> bool;
    auto WriteWordVideoController(uint32_t, uint32_t) -> bool;
    auto WriteWordMemcControl(const ControlWord&) -> bool;
    auto WriteWordDmaAddressGenerator(const DmaAddressGeneratorWord&) -> bool;
    auto WriteWordAddressTranslator(const AddressTranslation&) -> bool;

    auto WriteByteLogical(uint32_t, uint32_t) -> bool;
    auto WriteByteLogical(uint32_t, uint32_t, uint32_t) -> bool;
    auto WriteBytePhysical(uint32_t, uint32_t) -> bool;
    auto WriteByteInputOutputController(uint32_t, uint32_t) -> bool;
    auto WriteByteVideoController(uint32_t, uint32_t) -> bool;
    auto WriteByteMemcControl(const ControlWord&) -> bool;
    auto WriteByteDmaAddressGenerator(const DmaAddressGeneratorWord&) -> bool;
    auto WriteByteAddressTranslator(const AddressTranslation&) -> bool;

protected:
    testing::NiceMock<ArchimedesMock> mediator;

private:
    rbrown::acorn::archimedes::ArchimedesMemc memc;
};
