#include "Archimedes/ArchimedesVidc.h"
#include "Archimedes/ArchimedesMediator.h"

#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

constexpr auto ValueWithinRange(uint32_t v, uint32_t min, uint32_t max) {
    return (v >= min) && (v < max);
}

constexpr std::array PIXEL_RATE_FREQUENCIES = { 8u, 12u, 16u, 24u };

constexpr auto MapLogicalColour(uint32_t d) {
    const auto r = ExtractBitField(d, 0u, 4u);
    const auto g = ExtractBitField(d, 4u, 4u);
    const auto b = ExtractBitField(d, 8u, 4u);
    const auto t = (r << 16u) + (g << 8u) + b;
    return 17u * t;
}

constexpr auto MapLogicalColour(uint32_t d, uint32_t l) {
    const auto b = ExtractBitField(l, 7u, 1u);
    const auto g = ExtractBitField(l, 5u, 2u);
    const auto r = ExtractBitField(l, 4u, 1u);
    const auto t = (b << 11u) + (g << 6u) + (r << 3u);
    return MapLogicalColour(t + (d & 0x737u));
}

constexpr auto MapDisplayPeriodToPixels(uint32_t d, uint32_t bpp) {
    return 2u * d + 3u + (16u >> bpp);
}

namespace rbrown::acorn::archimedes {

ArchimedesVidc::ArchimedesVidc(ArchimedesMediator& m) :
    mediator{m},
    logicalColourRegisters{},
    borderColour{},
    stereoImageRegisters{},
    horizontalCyclePixels{ 1024u },
    horizontalSyncWidthPixels{},
    horizontalBorderStartPixels{},
    horizontalDisplayStartPixels{},
    horizontalDisplayStart{},
    horizontalDisplayEndPixels{},
    horizontalDisplayEnd{},
    horizontalBorderEndPixels{},
    horizontalCursorStartPixels{},
    horizontalInterlace{},
    verticalCycleRasters{ 313u },
    verticalSyncWidthRasters{},
    verticalBorderStartRasters{},
    verticalDisplayStartRasters{},
    verticalDisplayEndRasters{},
    verticalBorderEndRasters{},
    verticalCursorStartRasters{},
    verticalCursorEndRasters{},
    soundSamplePeriod{},
    controlTestMode0{},
    controlTestMode1{},
    controlCompositeSync{},
    controlInterlaceSync{},
    controlDmaRequest{ 3u },
    controlBitsPerPixel{},
    controlPixelRate{},
    controlPixelRateFrequency{ PIXEL_RATE_FREQUENCIES[controlPixelRate] },
    pixelMilliTicks{},
    currentPixel{},
    currentRaster{},
    videoFifo{ m },
    cursorFifo{ m },
    videoColourTable{},
    cursorColourTable{} {}

auto ArchimedesVidc::WriteWordVideo(uint32_t v) -> void { videoFifo.WriteWord(v); }
auto ArchimedesVidc::WriteWordCursor(uint32_t v) -> void { cursorFifo.WriteWord(v); }

auto ArchimedesVidc::WriteWordRegister(uint32_t v) -> void {
    const auto address = ExtractBitField(v, 24u, 8u);
    switch(address) {
        case 0x00u: return WriteLogicalColourRegister(0x0u, v);
        case 0x04u: return WriteLogicalColourRegister(0x1u, v);
        case 0x08u: return WriteLogicalColourRegister(0x2u, v);
        case 0x0Cu: return WriteLogicalColourRegister(0x3u, v);
        case 0x10u: return WriteLogicalColourRegister(0x4u, v);
        case 0x14u: return WriteLogicalColourRegister(0x5u, v);
        case 0x18u: return WriteLogicalColourRegister(0x6u, v);
        case 0x1Cu: return WriteLogicalColourRegister(0x7u, v);
        case 0x20u: return WriteLogicalColourRegister(0x8u, v);
        case 0x24u: return WriteLogicalColourRegister(0x9u, v);
        case 0x28u: return WriteLogicalColourRegister(0xAu, v);
        case 0x2Cu: return WriteLogicalColourRegister(0xBu, v);
        case 0x30u: return WriteLogicalColourRegister(0xCu, v);
        case 0x34u: return WriteLogicalColourRegister(0xDu, v);
        case 0x38u: return WriteLogicalColourRegister(0xEu, v);
        case 0x3Cu: return WriteLogicalColourRegister(0xFu, v);
        case 0x40u: return WriteBorderColourRegister(v);
        case 0x44u: return WriteCursorColourRegister(1u, v);
        case 0x48u: return WriteCursorColourRegister(2u, v);
        case 0x4Cu: return WriteCursorColourRegister(3u, v);
        case 0x60u: return WriteStereoImageRegister(7u, v);
        case 0x64u: return WriteStereoImageRegister(0u, v);
        case 0x68u: return WriteStereoImageRegister(1u, v);
        case 0x6Cu: return WriteStereoImageRegister(2u, v);
        case 0x70u: return WriteStereoImageRegister(3u, v);
        case 0x74u: return WriteStereoImageRegister(4u, v);
        case 0x78u: return WriteStereoImageRegister(5u, v);
        case 0x7Cu: return WriteStereoImageRegister(6u, v);
        case 0x80u: return WriteHorizontalCycleRegister(v);
        case 0x84u: return WriteHorizontalSyncWidthRegister(v);
        case 0x88u: return WriteHorizontalBorderStartRegister(v);
        case 0x8Cu: return WriteHorizontalDisplayStartRegister(v);
        case 0x90u: return WriteHorizontalDisplayEndRegister(v);
        case 0x94u: return WriteHorizontalBorderEndRegister(v);
        case 0x98u: return WriteHorizontalCursorStartRegister(v);
        case 0x9Cu: return WriteHorizontalInterlaceRegister(v);
        case 0xA0u: return WriteVerticalCycleRegister(v);
        case 0xA4u: return WriteVerticalSyncWidthRegister(v);
        case 0xA8u: return WriteVerticalBorderStartRegister(v);
        case 0xACu: return WriteVerticalDisplayStartRegister(v);
        case 0xB0u: return WriteVerticalDisplayEndRegister(v);
        case 0xB4u: return WriteVerticalBorderEndRegister(v);
        case 0xB8u: return WriteVerticalCursorStartRegister(v);
        case 0xBCu: return WriteVerticalCursorEndRegister(v);
        case 0xC0u: return WriteSoundFrequencyRegister(v);
        case 0xE0u: return WriteControlRegister(v);
        [[unlikely]] default: {
            spdlog::warn("VIDC: Unsupported register write {:02x}", address);
        }
    }
}

auto ArchimedesVidc::WriteLogicalColourRegister(uint32_t r, uint32_t d) -> void {
    logicalColourRegisters[r] = d;
    UpdateVideoColourTable(r, d);
}

auto ArchimedesVidc::UpdateVideoColourTable(uint32_t r, uint32_t d) -> void {
    if (controlBitsPerPixel == BPP_8) {
        for (auto i = 0u; i < 16u; ++i) {
            const auto index = (i << 4u) + r;
            videoColourTable[index] = MapLogicalColour(d, index);
        }
        return;
    }
    videoColourTable[r] = MapLogicalColour(d);
}

auto ArchimedesVidc::RebuildVideoColourTable() -> void {
    for (auto r = 0u; r < 16u; ++r) {
        UpdateVideoColourTable(r, logicalColourRegisters[r]);
    }
}

auto ArchimedesVidc::WriteBorderColourRegister(uint32_t d) -> void {
    borderColour = MapLogicalColour(d);
}

auto ArchimedesVidc::WriteCursorColourRegister(uint32_t r, uint32_t d) -> void {
    cursorColourTable[r] = MapLogicalColour(d);
}

auto ArchimedesVidc::WriteStereoImageRegister(uint32_t r, uint32_t d) -> void {
    stereoImageRegisters[r] = d & 0x7u;
}

auto ArchimedesVidc::WriteHorizontalCycleRegister(uint32_t d) -> void {
    // The VTI manual defines this as the period, in units of two pixels, of the horizontal scan.
    // Horizontal scan meaning the display time + horizontal retrace time
    // If N pixels are required then a value of (N - 2) / 2 should be written to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalCyclePixels = 2u * (field + 1u);
}

auto ArchimedesVidc::WriteHorizontalSyncWidthRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of two pixels, of the horizontal sync pulse
    // If N pixels are required then a value of (N - 2) / 2 should be written to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalSyncWidthPixels = 2u * (field + 1u);
}

auto ArchimedesVidc::WriteHorizontalBorderStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the time, in units of two pixels, from the start of the -HSYNC pulse
    // to the start of the border display
    // If M pixels are required then a value of (M - 1) / 2 should be written to this register
    // If a border is not required then set this to the same value os the horizontal display start register
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalBorderStartPixels = 2u * field + 1u;
}

auto ArchimedesVidc::WriteHorizontalDisplayStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the time, in units of two pixels, from the start of the -HSYNC pulse
    // to the start of the video display
    // The value written to this register is dependent on the number of bits per pixel
    // If M pixels are required then write (M - 5) / 2, (M - 7) / 2, (M - 11) / 2 or (M - 19) / 2
    // respectively in the 8bpp, 4bpp, 2bpp and 1bpp modes
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalDisplayStartPixels = MapDisplayPeriodToPixels(field, controlBitsPerPixel);
    horizontalDisplayStart = d;
}

auto ArchimedesVidc::WriteHorizontalDisplayEndRegister(uint32_t d) -> void {
    // The VTI manual defines this as the time, in units of two pixels, from the start of the -HSYNC pulse
    // to the start of the video display
    // The value written to this register is dependent on the number of bits per pixel
    // If M pixels are required then write (M - 5) / 2, (M - 7) / 2, (M - 11) / 2 or (M - 19) / 2
    // respectively in the 8bpp, 4bpp, 2bpp and 1bpp modes
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalDisplayEndPixels = MapDisplayPeriodToPixels(field, controlBitsPerPixel);
    horizontalDisplayEnd = d;
}

auto ArchimedesVidc::WriteHorizontalBorderEndRegister(uint32_t d) -> void {
    // The VTI manual defines this as the time, in units of two pixels, from the start of the -HSYNC pulse
    // to the end of the border display
    // If M pixels are required then a value of (M - 1) / 2 should be written to this register
    // If a border is not required then set this to the same value os the horizontal display end register
    const auto field = ExtractBitField(d, 14u, 10u);
    horizontalBorderEndPixels = 2u * field + 1u;
}

auto ArchimedesVidc::WriteHorizontalCursorStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the time, in units of single pixels, from the start of the -HSYNC pulse
    // to the start of the cursor display
    // If M pixels are required then a value of (M - 6) / 2 should be written to this register
    // Note: That this register may require special treatment in high resolution modes
    // Refer to the VTI manual for details
    const auto field = ExtractBitField(d, 13u, 11u);
    horizontalCursorStartPixels = field + 6u;
}

auto ArchimedesVidc::WriteHorizontalInterlaceRegister(uint32_t d) -> void {
    // The VTI manual says this register must be programmed if an interlaced sync display is required.
    // Otherwise, this register may be ignored
    // If a value L is written to the Horizontal Cycle Register the value (L + 1) / 2 should be
    // written to this register
    horizontalInterlace = ExtractBitField(d, 14u, 10u);
}

auto ArchimedesVidc::WriteVerticalCycleRegister(uint32_t d) -> void {
    // The VTI manual defines this as the period, in units of a raster, of the vertical scan
    // i.e. display time + fly back time
    // If N rasters are required write N - 1 to this register, unless an interlaced display is being
    // used then write (N - 3) / 2
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalCycleRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalSyncWidthRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, of the -V/CS pulse
    // If N rasters are required write N - 1 to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalSyncWidthRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalBorderStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the start of the border display
    // If N rasters are required write N - 1 to this register
    // If no border is required then this register must have the same value as the
    // Vertical Display Start Register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalBorderStartRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalDisplayStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the start of the display
    // If N rasters are required write N - 1 to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalDisplayStartRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalDisplayEndRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the end of the display
    // If N rasters are required write N - 1 to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalDisplayEndRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalBorderEndRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the end of the border display
    // If N rasters are required write N - 1 to this register
    // If no border is required then this register must have the same value as the
    // Vertical Display End Register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalBorderEndRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalCursorStartRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the start of the cursor display
    // If N rasters are required write N - 1 to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalCursorStartRasters = field + 1u;
}

auto ArchimedesVidc::WriteVerticalCursorEndRegister(uint32_t d) -> void {
    // The VTI manual defines this as the width, in units of a raster, from the start of the vertical
    // sync pulse to the end of the cursor display
    // If N rasters are required write N - 1 to this register
    const auto field = ExtractBitField(d, 14u, 10u);
    verticalCursorEndRasters = field + 1u;
}

auto ArchimedesVidc::WriteSoundFrequencyRegister(uint32_t d) -> void {
    // The VTI manual defines this byte sample rate of the sound data
    // If a period of N microseconds is required write N - 1 to this register
    // N may be between 3 and 256
    // Bit 8 of this register is significant since when it is clear all internal timings are cleared
    const auto field = ExtractBitField(d, 0u, 8u);
    soundSamplePeriod = field + 1u;
}

auto ArchimedesVidc::WriteControlRegister(uint32_t d) -> void {
    const auto oldBitsPerPixel = controlBitsPerPixel;
    const auto oldDmaRequest = controlDmaRequest;
    controlTestMode0 = ExtractBitField(d, 14u, 2u);
    controlTestMode1 = ExtractBitField(d, 8u, 1u);
    controlCompositeSync = ExtractBitField(d, 7u, 1u);
    controlInterlaceSync = ExtractBitField(d, 6u, 1u);
    controlDmaRequest = ExtractBitField(d, 4u, 2u);
    controlBitsPerPixel = ExtractBitField(d, 2u, 2u);
    controlPixelRate = ExtractBitField(d, 0u, 2u);
    controlPixelRateFrequency = PIXEL_RATE_FREQUENCIES[controlPixelRate];
    if (oldBitsPerPixel != controlBitsPerPixel) {
        RebuildVideoColourTable();
        WriteHorizontalDisplayStartRegister(horizontalDisplayStart);
        WriteHorizontalDisplayEndRegister(horizontalDisplayEnd);
        videoFifo.SetBitsPerPixel(controlBitsPerPixel);
    }
    if (oldDmaRequest != controlDmaRequest) {
        videoFifo.SetDmaRequest(controlDmaRequest);
    }
}

auto ArchimedesVidc::PixelIsInsideBorder(uint32_t pixel, uint32_t raster) const -> bool {
    return PixelIsInsideBorderHorizontalLimits(pixel) && RasterIsInsideBorderVerticalLimits(raster);
}

auto ArchimedesVidc::PixelIsInsideBorderHorizontalLimits(uint32_t pixel) const -> bool {
    return ValueWithinRange(pixel, horizontalBorderStartPixels, horizontalBorderEndPixels);
}

auto ArchimedesVidc::RasterIsInsideBorderVerticalLimits(uint32_t raster) const -> bool {
    return ValueWithinRange(raster, verticalBorderStartRasters, verticalBorderEndRasters);
}

auto ArchimedesVidc::PixelIsInsideDisplay(uint32_t pixel, uint32_t raster) const -> bool {
    return PixelIsInsideDisplayHorizontalLimits(pixel) && RasterIsInsideDisplayVerticalLimits(raster);
}

auto ArchimedesVidc::PixelIsInsideDisplayHorizontalLimits(uint32_t pixel) const -> bool {
    return ValueWithinRange(pixel, horizontalDisplayStartPixels, horizontalDisplayEndPixels);
}

auto ArchimedesVidc::RasterIsInsideDisplayVerticalLimits(uint32_t raster) const -> bool {
    return ValueWithinRange(raster, verticalDisplayStartRasters, verticalDisplayEndRasters);
}

auto ArchimedesVidc::RasterIsFirstOfDisplay(uint32_t raster) const -> bool {
    return raster == verticalDisplayStartRasters && RasterIsInsideDisplayVerticalLimits(raster);
}

auto ArchimedesVidc::RasterIsFirstAfterDisplay(uint32_t raster) const -> bool {
    return raster == verticalDisplayEndRasters && !RasterIsInsideDisplayVerticalLimits(raster);
}

auto ArchimedesVidc::PixelIsInsideCursor(uint32_t pixel, uint32_t raster) const -> bool {
    return PixelIsInsideCursorHorizontalLimits(pixel) && RasterIsInsideCursorVerticalLimits(raster);
}

auto ArchimedesVidc::PixelIsInsideCursorHorizontalLimits(uint32_t pixel) const -> bool {
    return ValueWithinRange(pixel, horizontalCursorStartPixels, horizontalCursorStartPixels + 32u);
}

auto ArchimedesVidc::RasterIsInsideCursorVerticalLimits(uint32_t raster) const -> bool {
    return ValueWithinRange(raster, verticalCursorStartRasters, verticalCursorEndRasters);
}

auto ArchimedesVidc::RasterIsEvenCursorRaster(uint32_t raster) const -> bool {
    return ~(raster - verticalCursorStartRasters) & 1u;
}

auto ArchimedesVidc::PixelIsInsideHSync(uint32_t pixel) const -> bool {
    return pixel < horizontalSyncWidthPixels;
}

auto ArchimedesVidc::EndFrame() -> void {
    if (horizontalDisplayEndPixels > horizontalDisplayStartPixels &&
        verticalDisplayEndRasters > verticalDisplayStartRasters) {
        mediator.SetTextureSource(
            0u,
            0u,
            horizontalDisplayEndPixels - horizontalDisplayStartPixels,
            verticalDisplayEndRasters - verticalDisplayStartRasters);
    }
    mediator.FrameCompleted();
}

auto ArchimedesVidc::Update(uint32_t time) -> void {
    const auto milliTicks = controlPixelRateFrequency * time;
    const auto totalPixelMilliTicks = milliTicks + pixelMilliTicks;
    const auto ticks = totalPixelMilliTicks / 1000u;
    pixelMilliTicks = totalPixelMilliTicks - 1000u * ticks;
    for (auto i = 0u; i < ticks; ++i) {
        UpdatePixel();
    }
}

auto ArchimedesVidc::UpdatePixel() -> void {
    const auto inBorder = PixelIsInsideBorder(currentPixel, currentRaster);
    const auto inDisplay = PixelIsInsideDisplay(currentPixel, currentRaster);
    const auto inCursor = PixelIsInsideCursor(currentPixel, currentRaster);
    if (inBorder) {
        if (inDisplay) {
            const auto displayIndex = videoFifo.ReadNext();
            const auto cursorIndex = inCursor ? cursorFifo.ReadNext() : 0u;
            const auto colour = cursorIndex ?
                cursorColourTable[cursorIndex] :
                videoColourTable[displayIndex];
            mediator.WritePixel(
                currentPixel - horizontalDisplayStartPixels,
                currentRaster - verticalDisplayStartRasters,
                colour);
        } else if (inCursor) {
            cursorFifo.ReadNext();
        }
    }
    if (++currentPixel < horizontalCyclePixels) {
        if (RasterIsFirstOfDisplay(currentRaster) &&
            PixelIsInsideHSync(currentPixel - 1u) &&
            !PixelIsInsideHSync(currentPixel)) {
            videoFifo.Reset();
        }
    } else {
        if (++currentRaster >= verticalCycleRasters) {
            EndFrame();
            currentRaster = 0u;
        }
        currentPixel = 0u;

        const auto firstPixelInHSync = PixelIsInsideHSync(0u);

        if (firstPixelInHSync &&
            RasterIsInsideCursorVerticalLimits(currentRaster) &&
            RasterIsEvenCursorRaster(currentRaster)) {
            cursorFifo.Reset();
        }

        if (RasterIsFirstOfDisplay(currentRaster)) {
            mediator.EndFlyBack();
            if (!firstPixelInHSync) {
               videoFifo.Reset();
            }
        } else if (RasterIsFirstAfterDisplay(currentRaster)) {
            mediator.StartFlyBack();
        }
    }
}

}