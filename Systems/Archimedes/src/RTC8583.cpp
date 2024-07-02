#include "Archimedes/RTC8583.h"
#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

namespace rbrown::acorn::archimedes {

RTC8583::RTC8583() :
    state{ State::Stopped },
    address{},
    ram{} {}

auto RTC8583::Start() -> void { state = State::Stopped; }
auto RTC8583::Stop() -> void { state = State::Stopped; }

auto RTC8583::ReceiveTargetAddress(uint8_t targetAddress) -> bool {
    const auto addressField = ExtractBitField(targetAddress, 1u, 7u) << 1u;
    const auto readField = ExtractBitField(targetAddress, 0u, 1u);
    if (state != State::Stopped || addressField != 0xA0u) {
        return false;
    }
    state = readField ?
            State::TransmittingData :
            State::ReceivingRegisterAddress;
    return true;
}

auto RTC8583::ReceiveData(uint8_t v) -> bool {
    if (state == State::ReceivingRegisterAddress) {
        address = v;
        state = State::ReceivingData;
        return true;
    } else if (state == State::ReceivingData) {
        ram[address++] = v;
        return true;
    }
    return false;
}

auto RTC8583::StartTransmit() -> uint8_t {
    if (address < 16u) {
        spdlog::warn("RTC8583 read from RTC clock register {:02x}", address);
    }
    return ram[address];
}

auto RTC8583::EndTransmit() -> bool {
    ++address;
    return true;
}

}