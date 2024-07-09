#pragma once

#include "Archimedes/I2CBus.h"

#include <array>

namespace rbrown::acorn::archimedes {

class RTC8583 : public I2CBus {
public:
    RTC8583();
    ~RTC8583() override = default;
    [[nodiscard]] auto ReadAddress() const -> uint8_t;
    [[nodiscard]] auto ReadRam(uint8_t) const -> uint8_t;
    auto WriteRam(uint8_t, uint8_t) -> void;
    auto Start() -> void override;
    auto Stop() -> void override;
    auto ReceiveTargetAddress(uint8_t) -> bool override;
    auto ReceiveData(uint8_t) -> bool override;
    auto StartTransmit(uint8_t&) -> bool override;
    auto EndTransmit() -> bool override;
private:
    enum class State : uint8_t {
        Stopped,
        ReceivingRegisterAddress,
        ReceivingData,
        TransmittingDataStart,
        TransmittingDataEnd
    };
    State state;
    uint8_t address;
    std::array<uint8_t, 256> ram;
};

}