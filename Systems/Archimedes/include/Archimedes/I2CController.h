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

    [[nodiscard]] auto GetClock() const -> bool;
    [[nodiscard]] auto GetData() const -> bool;
    [[nodiscard]] auto GetDataValid() const -> bool;
    [[nodiscard]] auto GetTransmit() const -> bool;
    [[nodiscard]] auto GetState() const -> State;
    [[nodiscard]] auto GetBuffer() const -> uint8_t;
    [[nodiscard]] auto GetTransferredBits() const -> uint8_t;

    auto SetClock(bool) -> void;
    auto SetData(bool) -> void;
    auto SetDataValid(bool) -> void;
    auto SetTransmit(bool) -> void;
    auto SetState(State) -> void;
    auto SetBuffer(uint8_t) -> void;
    auto SetTransferredBits(uint8_t) -> void;

    auto ReceiveBit() -> bool;
    auto TransmitBit() -> bool;

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