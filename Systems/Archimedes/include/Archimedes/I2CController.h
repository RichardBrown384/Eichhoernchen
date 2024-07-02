#pragma once

#include <cstdint>

namespace rbrown::acorn::archimedes {

class I2CBus;

class I2CController {
public:
    explicit I2CController(I2CBus&);
    [[nodiscard]] auto ReadData() const -> bool;
    auto WriteClockData(uint32_t) -> void;
private:
    enum class State {
        Stopped,
        ReceivingTargetAddress,
        ReceivingData,
        ReceivingAck,
        TransmittingData,
        TransmittingNack,
        TransmittingAckToReceive,
        TransmittingAckToTransmit
    };
    auto Update() -> I2CController::State;

    [[nodiscard]] auto GetState() const -> State;
    [[nodiscard]] auto GetBuffer() const -> uint8_t;

    auto SetState(State) -> void;
    auto SetBuffer(uint8_t) -> void;

    auto ReceiveBit() -> bool;
    auto TransmitBit() -> bool;
    auto SetTransmit(bool) -> void;

    bool clock;
    bool data;
    bool dataValid;
    bool transmit;
    State state;
    uint8_t buffer;
    uint8_t transferredBits;
    I2CBus& bus;
};

}