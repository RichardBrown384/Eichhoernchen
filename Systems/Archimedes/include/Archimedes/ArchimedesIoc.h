#pragma once

#include "Arm/Arm.h"

#include "Archimedes/ArchimedesTimer.h"
#include "Archimedes/ArchimedesKeyboard.h"
#include "Common/Util/BitUtil.h"
#include "Archimedes/I2CController.h"

namespace rbrown::acorn::archimedes {

class ArchimedesMediator;

class IrqRegister {
public:
    explicit IrqRegister(uint8_t s) : status{ s }, mask{ } {}

    [[nodiscard]] auto ReadStatus() const -> uint8_t { return status; }
    [[nodiscard]] auto ReadMask() const -> uint8_t { return mask; }
    [[nodiscard]] auto ReadRequest() const -> uint8_t { return mask & status; }

    auto WriteMask(uint32_t v) -> void { mask = v; }
    auto WriteClear(uint32_t v) -> void { ClearBits(v); }

    auto ClearBits(uint32_t v) -> void { status &= ~v; }
    auto SetBits(uint32_t v) -> void { status |= v; }
    auto ReplaceBits(uint32_t bit, uint32_t width, uint32_t replacement) {
        status = ReplaceBitField(status, bit, width, replacement);
    }

    [[nodiscard]] auto TestBits(uint32_t v) const -> bool { return status & v; }

private:
    uint8_t status;
    uint8_t mask;
};


class ArchimedesIoc : public rbrown::arm::Interrupts {
public:
    explicit ArchimedesIoc(ArchimedesMediator&);
    ~ArchimedesIoc() override = default;

    auto Reset() -> void override;

    auto PendingIrq() -> bool override;
    auto PendingFiq() -> bool override;

    auto Update(uint32_t) -> void;

    auto StartFlyBack() -> void;
    auto EndFlyBack() -> void;

    auto StartSoundInterrupt() -> void;
    auto EndSoundInterrupt() -> void;

    auto StartFloppyDiscDataRequest() -> void;
    auto StartFloppyDiscInterruptRequest() -> void;

    auto EndFloppyDiscDataRequest() -> void;
    auto EndFloppyDiscInterruptRequest() -> void;

    auto Read(uint32_t) -> uint32_t;
    auto Write(uint32_t, uint32_t) -> void;

private:
    [[nodiscard]] auto GetKeyboardByteTransmissionPeriod() const -> uint32_t;

    auto ReadInternalRegister(uint32_t) -> uint32_t;
    [[nodiscard]] auto ReadControlRegister() const -> uint32_t;
    auto ReadSerialReceiveRegister() -> uint32_t;

    auto ReadBank1(uint32_t) -> uint32_t;

    auto WriteInternalRegister(uint32_t, uint32_t) -> void;
    auto WriteControlRegister(uint32_t) -> void;
    auto WriteSerialTransmitRegister(uint32_t) -> void;

    auto WriteBank1(uint32_t, uint32_t) -> void;
    auto WriteBank5(uint32_t, uint32_t) -> void;

    auto WriteLatchA(uint32_t) -> void;
    auto WriteLatchB(uint32_t) -> void;

    uint8_t controlRegister;
    uint8_t serialTransmitRegister;
    uint8_t serialReceiveRegister;
    uint8_t serialReceiveBuffer;

    IrqRegister irqA;
    IrqRegister irqB;
    IrqRegister fiq;

    uint8_t latchA;
    uint8_t latchB;

    ReloadingTimer<1000u> timer0;
    ReloadingTimer<1000u> timer1;
    ReloadingTimer<1000u> timer2;
    ReloadingTimer<1000u> timer3;
    OneShotTimer<1000u> serialTransmitTimer;
    OneShotTimer<1000u> serialReceiveTimer;

    ArchimedesMediator& mediator;
};

}