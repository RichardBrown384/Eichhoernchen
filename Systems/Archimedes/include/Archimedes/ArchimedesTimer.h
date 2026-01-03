#pragma once

#include <cstdint>

namespace rbrown::acorn::archimedes {

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