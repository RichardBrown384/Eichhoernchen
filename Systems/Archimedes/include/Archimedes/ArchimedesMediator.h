#pragma once

#include <cstdint>

namespace rbrown::acorn::archimedes {

class ArchimedesMediator {
public:
    virtual ~ArchimedesMediator() = default;

    // Render Context Interface
    virtual auto FrameCompleted() -> void = 0;
    virtual auto WritePixel(uint32_t, uint32_t, uint32_t) -> void = 0;
    virtual auto SetTextureSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void = 0;

    // IOC Interface
    virtual auto ReadWordIoc(uint32_t) -> uint32_t = 0;
    virtual auto WriteWordIoc(uint32_t, uint32_t) -> void = 0;

    // VIDC Interface
    virtual auto WriteWordVidcRegister(uint32_t) -> void = 0;
    virtual auto WriteWordVidcVideo(uint32_t) -> void = 0;
    virtual auto WriteWordVidcCursor(uint32_t) -> void = 0;
    virtual auto WriteWordVidcSound(uint32_t) -> void = 0;

    // Keyboard Interface
    virtual auto ReadByteKeyboard(uint8_t& v) -> bool = 0;
    virtual auto WriteByteKeyboard(uint8_t) -> void = 0;

    // I2C Interface
    virtual auto ReadDataI2C() -> bool = 0;
    virtual auto WriteClockDataI2C(uint32_t) -> void = 0;

    // Floppy disc events
    virtual auto ReadByteFloppyDiscController(uint32_t) -> uint8_t = 0;
    virtual auto WriteByteFloppyDiscController(uint32_t, uint8_t) -> void = 0;

    // Latch A/B
    virtual auto WriteByteLatchA(uint8_t) -> void = 0;
    virtual auto WriteByteLatchB(uint8_t) -> void = 0;

    // System Events
    virtual auto Update(uint32_t) -> void = 0;

    virtual auto StartFlyBack() -> void = 0;
    virtual auto EndFlyBack() -> void = 0;

    virtual auto StartSoundInterrupt() -> void = 0;
    virtual auto EndSoundInterrupt() -> void = 0;

    virtual auto StartVideoDmaRequest() -> void = 0;
    virtual auto StartCursorDmaRequest() -> void = 0;
    virtual auto StartSoundDmaRequest() -> void = 0;

    virtual auto EndVideoDmaRequest() -> void = 0;
    virtual auto EndCursorDmaRequest() -> void = 0;
    virtual auto EndSoundDmaRequest() -> void = 0;

    virtual auto StartFloppyDiscDataRequest() -> void = 0;
    virtual auto StartFloppyDiscInterruptRequest() -> void = 0;

    virtual auto EndFloppyDiscDataRequest() -> void = 0;
    virtual auto EndFloppyDiscInterruptRequest() -> void = 0;
};

}
