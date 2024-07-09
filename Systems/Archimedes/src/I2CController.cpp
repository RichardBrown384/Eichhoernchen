#include "Archimedes/I2CController.h"
#include "Archimedes/I2CBus.h"
#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

namespace rbrown::acorn::archimedes {

I2CController::I2CController(I2CBus& b) :
    clock{ true },
    data{ true },
    dataValid{ false },
    transmit{ true },
    state{ State::Stopped },
    buffer{},
    transferredBits{},
    bus{ b } {}

auto I2CController::GetClock() const -> bool { return clock; }
auto I2CController::GetData() const -> bool { return data; }
auto I2CController::GetDataValid() const -> bool { return dataValid; }
auto I2CController::GetTransmit() const -> bool { return transmit; }
auto I2CController::GetState() const -> State { return state; }
auto I2CController::GetBuffer() const -> uint8_t { return buffer; }
auto I2CController::GetTransferredBits() const -> uint8_t { return transferredBits; }

auto I2CController::SetClock(bool v) -> void { clock = v; }
auto I2CController::SetData(bool v) -> void { data = v; }
auto I2CController::SetDataValid(bool v) -> void { dataValid = v; }
auto I2CController::SetTransmit(bool v) -> void { transmit = v; }
auto I2CController::SetState(State s) -> void { state = s; }
auto I2CController::SetBuffer(uint8_t v) -> void {
    buffer = v;
    transferredBits = 0u;
}
auto I2CController::SetTransferredBits(uint8_t v) -> void { transferredBits = v; }

auto I2CController::ReadData() const -> bool { return GetData() && GetTransmit(); }

auto I2CController::WriteClockData(uint32_t v) -> void {
    const auto nextData = ExtractBitField(v, 0u, 1u);
    const auto nextClock = ExtractBitField(v, 1u, 1u);

    if (GetClock() == nextClock) {
        if (GetClock() && GetData() != nextData) {
            if (nextData) {
                spdlog::debug("I2C received STOP");
                bus.Stop();
                SetState(State::Stopped);
            } else {
                spdlog::debug("I2C received START");
                SetBuffer(0u);
                SetTransmit(true);
                bus.Start();
                SetState(State::ReceivingTargetAddress);
            }
            SetDataValid(false);
        }
    } else if (!nextClock) {
        SetTransmit(true);
        if (GetDataValid()) {
            const auto nextState = Update();
            if (nextState == State::Stopped) {
                bus.Stop();
            }
            SetState(nextState);
        }
        SetDataValid(true);
    }

    SetClock(nextClock);
    SetData(nextData);
}

auto I2CController::Update() -> I2CController::State {
    switch (GetState()) {
        case State::ReceivingTargetAddress: {
            if (ReceiveBit()) {
                const auto v = GetBuffer();
                spdlog::debug("I2C received target address {:02x}", v);
                if (bus.ReceiveTargetAddress(v)) {
                    SetTransmit(false);
                    const auto transmitting = ExtractBitField(v, 0u, 1u);
                    return transmitting ?
                           State::TransmittingAckToTransmit :
                           State::TransmittingAckToReceive;
                }
                return State::TransmittingNack;
            }
            return State::ReceivingTargetAddress;
        }

        case State::ReceivingData: {
            if (ReceiveBit()) {
                const auto v = GetBuffer();
                spdlog::debug("I2C received data address {:02x}", v);
                if (bus.ReceiveData(v)) {
                    SetTransmit(false);
                    return State::TransmittingAckToReceive;
                }
                return State::TransmittingNack;
            }
            return State::ReceivingData;
        }

        case State::ReceivingAck: {
            spdlog::debug("I2C transmitted data");
            spdlog::debug("I2C Receiving ack for target to transmit {:02x}", GetData());
            uint8_t v;
            if (!GetData() && bus.StartTransmit(v)) {
                SetBuffer(v);
                TransmitBit();
                return State::TransmittingData;
            }
            return State::Stopped;
        }

        case State::TransmittingData: {
            if (TransmitBit()) {
                return bus.EndTransmit() ?
                       State::ReceivingAck :
                       State::Stopped;
            }
            return State::TransmittingData;
        }

        case State::TransmittingNack: {
            spdlog::debug("I2C nack transmitted");
            return State::Stopped;
        }

        case State::TransmittingAckToReceive: {
            spdlog::debug("I2C receive ack transmitted");
            return State::ReceivingData;
        }

        case State::TransmittingAckToTransmit: {
            spdlog::debug("I2C transmit ack transmitted");
            uint8_t v;
            if (bus.StartTransmit(v)) {
                SetBuffer(v);
                TransmitBit();
                return State::TransmittingData;
            }
            return State::Stopped;
        }

        default:
            return State::Stopped;
    }
}

auto I2CController::ReceiveBit() -> bool {
    const auto bits = GetTransferredBits() + 1u;
    const auto b = GetBuffer() << 1u;
    const auto d = GetData();
    SetBuffer(b + d);
    if (bits == 8u) {
        SetTransferredBits(0u);
        return true;
    }
    SetTransferredBits(bits);
    return false;
}

auto I2CController::TransmitBit() -> bool {
    const auto bits = GetTransferredBits() + 1u;
    const auto b = GetBuffer();
    SetTransmit(b & 0x80u);
    SetBuffer(b << 1u);
    if (bits == 8u) {
        SetTransferredBits(0u);
        return true;
    }
    SetTransferredBits(bits);
    return false;
}

}
