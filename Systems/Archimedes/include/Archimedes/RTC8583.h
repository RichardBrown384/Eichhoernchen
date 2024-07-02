#pragma once

#include "Archimedes/I2CBus.h"

#include <array>

namespace rbrown::acorn::archimedes {

class RTC8583 : public I2CBus {
public:
    RTC8583();
    ~RTC8583() override = default;
    auto Start() -> void override;
    auto Stop() -> void override;
    auto ReceiveTargetAddress(uint8_t) -> bool override;
    auto ReceiveData(uint8_t) -> bool override;
    auto StartTransmit() -> uint8_t override;
    auto EndTransmit() -> bool override;
private:
    enum class State {
        Stopped,
        ReceivingRegisterAddress,
        ReceivingData,
        TransmittingData,
    };
    State state;
    uint8_t address;
    std::array<uint8_t, 256> ram;
};

}