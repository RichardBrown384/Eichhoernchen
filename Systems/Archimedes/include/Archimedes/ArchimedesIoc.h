#pragma once

#include "Arm/Arm.h"

#include "Archimedes/ArchimedesKeyboard.h"
#include "Archimedes/ArchimedesTimer.h"
#include "Common/Util/BitUtil.h"

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

template<uint32_t Scale>
class IocTimer {
public:
    explicit IocTimer(uint32_t input) :
        inputLatch{ input },
        outputLatch{ input },
        counterTicks{ Scale * input } {}

    template<typename F>
    auto Update(const uint32_t elapsedTicks, const F& expiryCallback) -> void {
        // Counter has not yet expired:
        if (counterTicks > elapsedTicks) {
            counterTicks -= elapsedTicks;
            return;
        }

        // Counter has expired with non-zero input latch:
        // Mathematics for the expired case (elapsedTicks >= counterTicks):
        //
        // Let:
        //   delta = elapsedTicks - counterTicks
        //   delta = q * Latch + r
        //   q = delta / Latch
        //   r = delta % Latch   (0 <= r < Latch)
        //
        // We want the next reload after the last expiry:
        //
        //   counterTicks + Latch * (q + 1) - elapsedTicks
        //
        // Substitute elapsedTicks = counterTicks + q * Latch + r:
        //
        //   counterTicks + q * Latch + Latch
        // - counterTicks - q * Latch - r
        //
        // = Latch - r
        // = Latch - (elapsedTicks - counterTicks) % Latch

        if (const auto scaledInputTicks = Scale * inputLatch) {
            counterTicks = scaledInputTicks - (elapsedTicks - counterTicks) % scaledInputTicks;
            expiryCallback();
            return;
        }

        // In the case where the input latch was set to zero in the interim we invoke the callback
        // we then expire the counter. The documentation says the timer continuously reloads when the input latch
        // is zero, but it doesn't explicitly mention if the callback should be invoked or what reload actually
        // means, we assume here the counter is continually reloaded with zero and doesn't count down.
        if (counterTicks) {
            expiryCallback();
        }
        counterTicks = 0u;
    }

    [[nodiscard]] auto ReadOutputLatchLow() const -> uint32_t {
        return ExtractBitField(outputLatch, 0u, 8u);
    }
    [[nodiscard]] auto ReadOutputLatchHigh() const -> uint32_t {
        return ExtractBitField(outputLatch, 8u, 8u);
    }
    [[nodiscard]] auto ReadInputLatch() const -> uint32_t {
        return inputLatch;
    }

    auto WriteInputLatchLow(uint32_t v) -> void {
        inputLatch = ReplaceBitField(inputLatch, 0u, 8u, v);
    }
    auto WriteInputLatchHigh(uint32_t v) -> void {
        inputLatch = ReplaceBitField(inputLatch, 8u, 8u, v);
    }
    auto WriteGoCommand() -> void {
        counterTicks = Scale * inputLatch;
    }
    auto WriteLatchCommand() -> void {
        outputLatch = counterTicks / Scale;
    }

private:
    uint32_t inputLatch;
    uint32_t outputLatch;
    uint32_t counterTicks;
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

    IocTimer<1000u> timer0;
    IocTimer<1000u> timer1;
    IocTimer<1000u> timer2;
    IocTimer<1000u> timer3;
    OneShotTimer<1000u> serialTransmitTimer;
    OneShotTimer<1000u> serialReceiveTimer;

    ArchimedesMediator& mediator;
};

}