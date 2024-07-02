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

auto I2CController::ReadData() const -> bool { return data & transmit; }

auto I2CController::WriteClockData(uint32_t v) -> void {
    const auto nextData = ExtractBitField(v, 0u, 1u);
    const auto nextClock = ExtractBitField(v, 1u, 1u);

    if (clock == nextClock) {
        if (clock && data != nextData) {
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
            dataValid = false;
        }
    } else if (!nextClock) {
        SetTransmit(true);
        if (dataValid) {
            const auto newState = Update();
            if (newState == State::Stopped) {
                bus.Stop();
            }
            SetState(newState);
        }
        dataValid = true;
    }

    clock = nextClock;
    data = nextData;
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
            spdlog::debug("I2C Receiving ack for target to transmit {:02x}", data);
            if (!data) {
                SetBuffer(bus.StartTransmit());
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
            SetBuffer(bus.StartTransmit());
            TransmitBit();
            return State::TransmittingData;
        }

        default:
            return State::Stopped;
    }
}

auto I2CController::GetState() const -> State { return state; }
auto I2CController::SetState(State v) -> void { state = v; }

auto I2CController::GetBuffer() const -> uint8_t { return buffer; }
auto I2CController::SetBuffer(uint8_t v) -> void {
    buffer = v;
    transferredBits = 0u;
}

auto I2CController::ReceiveBit() -> bool {
    buffer <<= 1u;
    buffer += data;
    if (++transferredBits == 8u) {
        transferredBits = 0u;
        return true;
    }
    return false;
}

auto I2CController::TransmitBit() -> bool {
    SetTransmit(buffer & 0x80u);
    buffer <<= 1u;
    if (++transferredBits == 8u) {
        transferredBits = 0u;
        return true;
    }
    return false;
}

auto I2CController::SetTransmit(bool v) -> void {
    transmit = v;
}

}
