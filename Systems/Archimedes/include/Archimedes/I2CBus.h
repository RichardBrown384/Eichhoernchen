#pragma once

#include <cstdint>

namespace rbrown::acorn::archimedes {

class I2CBus {
public:
    virtual ~I2CBus() = default;
    virtual auto Start() -> void = 0;
    virtual auto Stop() -> void = 0;
    virtual auto ReceiveTargetAddress(uint8_t) -> bool = 0;
    virtual auto ReceiveData(uint8_t) -> bool = 0;
    virtual auto StartTransmit(uint8_t&) -> bool = 0;
    virtual auto EndTransmit() -> bool = 0;
};

}