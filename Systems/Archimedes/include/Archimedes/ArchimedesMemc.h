#pragma once

#include "Arm/Arm.h"

#include <cstdint>
#include <vector>
#include <optional>

namespace rbrown::acorn::archimedes {

class ArchimedesMediator;

class ArchimedesMemc final: public rbrown::arm::Memory {
public:
    ArchimedesMemc(ArchimedesMediator&, uint32_t, std::vector<uint8_t>, std::vector<uint8_t>);
    ~ArchimedesMemc() override = default;

    auto Reset() -> void override;

    auto CycleS() -> void override;
    auto CycleN() -> void override;
    auto CycleI(uint32_t) -> void override;

    auto ReadWord(uint32_t, uint32_t&) -> bool override;
    auto ReadByte(uint32_t, uint32_t&) -> bool override;

    auto WriteWord(uint32_t, uint32_t) -> bool override;
    auto WriteByte(uint32_t, uint32_t) -> bool override;

    auto SetSupervisor(bool) -> void override;
    auto SetRomContinuouslyEnabled(bool) -> void;

    auto EndFlyBack() -> void;
    auto StartVideoDmaRequest() -> void;
    auto StartCursorDmaRequest() -> void;
    auto StartSoundDmaRequest() -> void;
    auto EndVideoDmaRequest() -> void;
    auto EndCursorDmaRequest() -> void;
    auto EndSoundDmaRequest() -> void;

    [[nodiscard]] auto GetSupervisor() const -> bool override;
    [[nodiscard]] auto GetRomContinuouslyEnabled() const -> bool;

    [[nodiscard]] auto GetOperatingSystemMode() const -> bool;
    [[nodiscard]] auto GetSoundDmaControl() const -> bool;
    [[nodiscard]] auto GetVideoDmaControl() const -> bool;
    [[nodiscard]] auto GetDramRefreshControl() const -> uint32_t;
    [[nodiscard]] auto GetHighRomAccessTime() const -> uint32_t;
    [[nodiscard]] auto GetLowRomAccessTime() const -> uint32_t;
    [[nodiscard]] auto GetPageSize() const -> uint32_t;
    [[nodiscard]] auto GetPageSizeBytes() const -> uint32_t;

    [[nodiscard]] auto GetSequential() const -> bool;

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
    [[nodiscard]] auto GetOutstandingDmaRequests() const -> bool;

private:
    [[nodiscard]] auto IsCycleSequential(uint32_t) const -> bool;

    auto SetOperatingSystemMode(bool) -> void;
    auto SetSoundDmaControl(bool) -> void;
    auto SetVideoDmaControl(bool) -> void;
    auto SetDramRefreshControl(uint32_t) -> void;
    auto SetHighRomAccessTime(uint32_t) -> void;
    auto SetLowRomAccessTime(uint32_t) -> void;
    auto SetPageSize(uint32_t) -> void;

    auto SetSequential(bool) -> void;

    auto SetVideoInit(uint32_t) -> void;
    auto SetVideoStart(uint32_t) -> void;
    auto SetVideoEnd(uint32_t) -> void;
    auto SetCursorInit(uint32_t) -> void;
    auto SetSoundStart(uint32_t) -> void;
    auto SetSoundEndNext(uint32_t) -> void;
    auto SetSoundPointer() -> void;
    auto SetNextSoundBufferValid(bool) -> void;

    auto SetVideoPointer(uint32_t) -> void;
    auto SetCursorPointer(uint32_t) -> void;
    auto SetSoundPointer(uint32_t) -> void;

    auto SetVideoDmaRequests(uint32_t) -> void;
    auto SetCursorDmaRequests(uint32_t) -> void;
    auto SetSoundDmaRequests(uint32_t) -> void;

    auto UpdateRamAccess(uint32_t) -> void;
    auto UpdateIoControl() -> void;
    auto UpdateLowRom() -> void;
    auto UpdateHighRom() -> void;
    auto UpdateVidc() -> void;
    auto UpdateMemcControl() -> void;
    auto UpdateAddressTranslator() -> void;

    auto UpdateDma() -> void;
    auto UpdateVideoDma() -> void;
    auto UpdateCursorDma() -> void;
    auto UpdateSoundDma() -> void;

    auto ReadByteLogical(uint32_t, uint32_t&) -> bool;
    auto ReadBytePhysical(uint32_t, uint32_t&) -> bool;
    auto ReadByteIoControl(uint32_t, uint32_t&) -> bool;
    auto ReadByteLowRom(uint32_t, uint32_t&) -> bool;
    auto ReadByteHighRom(uint32_t, uint32_t&) -> bool;

    auto ReadWordLogical(uint32_t, uint32_t&) -> bool;
    auto ReadWordPhysical(uint32_t, uint32_t&)  -> bool;
    auto ReadWordIoControl(uint32_t, uint32_t&) -> bool;
    auto ReadWordLowRom(uint32_t, uint32_t&) -> bool;
    auto ReadWordHighRom(uint32_t, uint32_t&) -> bool;
    auto ReadWordDmaPhysicalS(uint32_t) -> uint32_t;
    auto ReadWordDmaPhysicalN(uint32_t) -> uint32_t;

    auto WriteByteLogical(uint32_t, uint32_t) -> bool;
    auto WriteBytePhysical(uint32_t, uint32_t) -> bool;
    auto WriteByteIoControl(uint32_t, uint32_t) -> bool;
    auto WriteByteVidc(uint32_t) -> bool;
    auto WriteByteMemcControl(uint32_t) -> bool;
    auto WriteByteAddressTranslator(uint32_t) -> bool;

    auto WriteWordLogical(uint32_t, uint32_t) -> bool;
    auto WriteWordPhysical(uint32_t, uint32_t) -> bool;
    auto WriteWordIoControl(uint32_t, uint32_t) -> bool;
    auto WriteWordVidc(uint32_t) -> bool;
    auto WriteWordMemcControl(uint32_t) -> bool;
    auto WriteWordAddressTranslator(uint32_t) -> bool;
    auto FindReplacementPhysicalPage(uint32_t, uint32_t) -> std::optional<uint32_t>;
    auto WriteAddressTranslator(uint32_t, uint32_t, uint32_t) -> void;
    auto ClearAddressTranslator(uint32_t) -> void;

    auto TranslateReadLogicalAddress(uint32_t, uint32_t&) -> bool;
    auto TranslateWriteLogicalAddress(uint32_t, uint32_t&) -> bool;

    // Control register
    bool supervisor;
    bool operatingSystemMode;
    bool soundDmaControl;
    bool videoDmaControl;
    uint32_t dramRefreshControl;
    uint32_t highRomAccessTime;
    uint32_t lowRomAccessTime;
    uint32_t pageSize;

    // Dma address generators
    uint32_t videoInit;
    uint32_t videoStart;
    uint32_t videoEnd;
    uint32_t cursorInit;
    uint32_t soundStart;
    uint32_t soundEndNext;
    uint32_t soundEndCurrent;
    bool nextSoundBufferValid;

    // Dma pointers
    uint32_t videoPointer;
    uint32_t cursorPointer;
    uint32_t soundPointer;

    // Dma request counts
    uint32_t videoDmaRequests;
    uint32_t cursorDmaRequests;
    uint32_t soundDmaRequests;

    // Memory access controls
    bool sequential;
    bool romContinuouslyEnabled;
    uint32_t lowRomAccessTimeNanoSeconds;
    uint32_t highRomAccessTimeNanoSeconds;

    // Logical memory mapping
    uint32_t readProtectionLevel;
    uint32_t writeProtectionLevel;
    std::vector<uint32_t> logicals; // physical to logical
    std::vector<uint32_t> physicals; // logical to physical
    std::vector<uint8_t> ram;

    // Roms
    std::vector<uint8_t> lowRom;
    std::vector<uint8_t> highRom;

    // Mediator
    ArchimedesMediator& mediator;
};

}