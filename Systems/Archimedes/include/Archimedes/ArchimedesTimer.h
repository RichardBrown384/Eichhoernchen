#pragma once

#include "Common/Util/BitUtil.h"

#include <cstdint>

namespace rbrown::acorn::archimedes {

template<uint32_t Scale>
class ReloadingTimer {
public:
    explicit ReloadingTimer(uint32_t input) :
        inputLatch{ input },
        outputLatch{ input },
        scaledValue{ Scale * input } {}

    template<typename F>
    auto Update(uint32_t scaledTicks, const F& expiryCallback) -> void {
        if (scaledTicks >= scaledValue) {
            const auto scaledInputLatch = Scale * inputLatch;
            while (scaledTicks >= scaledValue) {
                scaledValue += scaledInputLatch;
            }
            expiryCallback();
        }
        scaledValue -= scaledTicks;
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
        scaledValue = Scale * inputLatch;
    }
    auto WriteLatchCommand() -> void {
        outputLatch = scaledValue / Scale;
    }

private:
    uint32_t inputLatch;
    uint32_t outputLatch;
    uint32_t scaledValue;
};

template<uint32_t Scale>
class OneShotTimer {
public:
    OneShotTimer() : scaledValue{} {}

    template<typename F>
    auto Update(uint32_t scaledTicks, const F& expiryCallback) -> void {
        while (scaledValue && scaledTicks >= scaledValue) {
            scaledTicks = scaledTicks - scaledValue;
            scaledValue = 0u;
            expiryCallback();
        }
        if (scaledValue) {
            scaledValue -= scaledTicks;
        }
    }
    [[nodiscard]] auto Active() const -> bool { return scaledValue; }
    auto Go(uint32_t v) -> void {
        scaledValue = Scale * v;
    }
private:
    uint32_t scaledValue;
};

}