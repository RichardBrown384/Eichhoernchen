#pragma once

#include "Archimedes/I2CBus.h"

#include <array>
#include <vector>

namespace rbrown::acorn::archimedes {

class RTC8583 final : public I2CBus {
public:
    explicit RTC8583(std::vector<uint8_t>);
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
    [[nodiscard]] auto GetRam() const -> std::vector<uint8_t>;
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