#pragma once

#include <array>

namespace rbrown::acorn::archimedes {

class ArchimedesMediator;

class VideoFifo {
public:
    explicit VideoFifo(ArchimedesMediator&);
    auto Reset() -> void;
    auto SetBitsPerPixel(uint32_t) -> void;
    auto SetDmaRequest(uint32_t) -> void;
    auto WriteWord(uint32_t v) -> void;
    auto ReadNext() -> uint32_t;
private:
    std::array<uint32_t, 8> buffer;
    uint32_t readPos;
    uint32_t writePos;
    uint32_t dmaRequest;
    uint32_t bits;
    uint32_t mask;
    ArchimedesMediator& mediator;
};

class CursorFifo {
public:
    explicit CursorFifo(ArchimedesMediator&);
    auto Reset() -> void;
    auto WriteWord(uint32_t v) -> void;
    auto ReadNext() -> uint32_t;
private:
    static constexpr auto BITS = 2u;
    static constexpr auto MASK = (1u << BITS) - 1u;
    std::array<uint32_t, 4> buffer;
    uint32_t readPos;
    uint32_t writePos;
    ArchimedesMediator& mediator;
};

class ArchimedesVidc {
public:
    explicit ArchimedesVidc(ArchimedesMediator&);
    auto WriteWordVideo(uint32_t) -> void;
    auto WriteWordCursor(uint32_t) -> void;
    auto WriteWordRegister(uint32_t) -> void;
    auto Update(uint32_t) -> void;
private:
    constexpr static auto BPP_1 = 0u;
    constexpr static auto BPP_2 = 1u;
    constexpr static auto BPP_4 = 2u;
    constexpr static auto BPP_8 = 3u;

    auto WriteLogicalColourRegister(uint32_t, uint32_t) -> void;
    auto UpdateVideoColourTable(uint32_t, uint32_t) -> void;
    auto RebuildVideoColourTable() -> void;
    auto WriteBorderColourRegister(uint32_t) -> void;
    auto WriteCursorColourRegister(uint32_t, uint32_t) -> void;

    auto WriteStereoImageRegister(uint32_t, uint32_t) -> void;

    auto WriteHorizontalCycleRegister(uint32_t) -> void;
    auto WriteHorizontalSyncWidthRegister(uint32_t) -> void;
    auto WriteHorizontalBorderStartRegister(uint32_t) -> void;
    auto WriteHorizontalDisplayStartRegister(uint32_t) -> void;
    auto WriteHorizontalDisplayEndRegister(uint32_t) -> void;
    auto WriteHorizontalBorderEndRegister(uint32_t) -> void;
    auto WriteHorizontalCursorStartRegister(uint32_t) -> void;
    auto WriteHorizontalInterlaceRegister(uint32_t) -> void;

    auto WriteVerticalCycleRegister(uint32_t) -> void;
    auto WriteVerticalSyncWidthRegister(uint32_t) -> void;
    auto WriteVerticalBorderStartRegister(uint32_t) -> void;
    auto WriteVerticalDisplayStartRegister(uint32_t) -> void;
    auto WriteVerticalDisplayEndRegister(uint32_t) -> void;
    auto WriteVerticalBorderEndRegister(uint32_t) -> void;
    auto WriteVerticalCursorStartRegister(uint32_t) -> void;
    auto WriteVerticalCursorEndRegister(uint32_t) -> void;

    auto WriteSoundFrequencyRegister(uint32_t) -> void;

    auto WriteControlRegister(uint32_t) -> void;

    [[nodiscard]] auto PixelIsInsideBorder(uint32_t, uint32_t) const -> bool;
    [[nodiscard]] auto PixelIsInsideBorderHorizontalLimits(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsInsideBorderVerticalLimits(uint32_t) const -> bool;

    [[nodiscard]] auto PixelIsInsideDisplay(uint32_t, uint32_t) const -> bool;
    [[nodiscard]] auto PixelIsInsideDisplayHorizontalLimits(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsInsideDisplayVerticalLimits(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsFirstOfDisplay(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsFirstAfterDisplay(uint32_t) const -> bool;

    [[nodiscard]] auto PixelIsInsideCursor(uint32_t, uint32_t) const -> bool;
    [[nodiscard]] auto PixelIsInsideCursorHorizontalLimits(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsInsideCursorVerticalLimits(uint32_t) const -> bool;
    [[nodiscard]] auto RasterIsEvenCursorRaster(uint32_t) const -> bool;

    [[nodiscard]] auto PixelIsInsideHSync(uint32_t) const -> bool;

    auto EndFrame() -> void;

    auto UpdatePixel() -> void;

    ArchimedesMediator& mediator;

    std::array<uint32_t, 16> logicalColourRegisters;

    uint32_t borderColour;

    std::array<uint32_t, 8> stereoImageRegisters;

    uint32_t horizontalCyclePixels;
    uint32_t horizontalSyncWidthPixels;
    uint32_t horizontalBorderStartPixels;
    uint32_t horizontalDisplayStartPixels;
    uint32_t horizontalDisplayStart;
    uint32_t horizontalDisplayEndPixels;
    uint32_t horizontalDisplayEnd;
    uint32_t horizontalBorderEndPixels;
    uint32_t horizontalCursorStartPixels;
    uint32_t horizontalInterlace;

    uint32_t verticalCycleRasters;
    uint32_t verticalSyncWidthRasters;
    uint32_t verticalBorderStartRasters;
    uint32_t verticalDisplayStartRasters;
    uint32_t verticalDisplayEndRasters;
    uint32_t verticalBorderEndRasters;
    uint32_t verticalCursorStartRasters;
    uint32_t verticalCursorEndRasters;

    uint32_t soundSamplePeriod;

    uint32_t controlTestMode0;
    uint32_t controlTestMode1;
    uint32_t controlCompositeSync;
    uint32_t controlInterlaceSync;
    uint32_t controlDmaRequest;
    uint32_t controlBitsPerPixel;
    uint32_t controlPixelRate;
    uint32_t controlPixelRateFrequency;

    uint32_t pixelMilliTicks;
    uint32_t currentPixel;
    uint32_t currentRaster;
    VideoFifo videoFifo;
    CursorFifo cursorFifo;

    std::array<uint32_t, 256> videoColourTable;
    std::array<uint32_t, 4> cursorColourTable;
};

}