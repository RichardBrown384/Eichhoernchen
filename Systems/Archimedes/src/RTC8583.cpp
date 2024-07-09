#include "Archimedes/RTC8583.h"
#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

namespace rbrown::acorn::archimedes {

RTC8583::RTC8583() :
    state{ State::Stopped },
    address{},
    ram{} {}

auto RTC8583::ReadAddress() const -> uint8_t { return address; }
auto RTC8583::ReadRam(uint8_t a) const -> uint8_t { return ram[a]; }
auto RTC8583::WriteRam(uint8_t a, uint8_t v) -> void { ram[a] = v; }

auto RTC8583::Start() -> void { state = State::Stopped; }
auto RTC8583::Stop() -> void { state = State::Stopped; }

auto RTC8583::ReceiveTargetAddress(uint8_t targetAddress) -> bool {
    const auto addressField = ExtractBitField(targetAddress, 1u, 7u) << 1u;
    const auto readField = ExtractBitField(targetAddress, 0u, 1u);
    if (state != State::Stopped) {
        spdlog::debug("RTC8583 target address received when not stopped {:02x}.", targetAddress);
        return false;
    }
    if (addressField != 0xA0u) {
        spdlog::debug("RTC8583 unexpected target address received {:02x}.", targetAddress);
        return false;
    }
    state = readField ?
            State::TransmittingDataStart :
            State::ReceivingRegisterAddress;
    return true;
}

auto RTC8583::ReceiveData(uint8_t v) -> bool {
    if (state == State::ReceivingRegisterAddress) {
        address = v;
        state = State::ReceivingData;
        return true;
    } else if (state == State::ReceivingData) {
        if (address < 16u) {
            spdlog::debug("RTC8583 write to {:02x} ({:02x})", address, v);
        }
        ram[address++] = v;
        return true;
    }
    spdlog::debug("RTC8583 unexpected data received {:02x}.", v);
    return false;
}

auto RTC8583::StartTransmit(uint8_t& v) -> bool {
    if (state == State::TransmittingDataStart) {
        if (address < 16u) {
            spdlog::debug("RTC8583 read from {:02x} ({:02x})", address, ram[address]);
        }
        v = ram[address];
        state = State::TransmittingDataEnd;
        return true;
    }
    spdlog::debug("RTC8583 unexpected start transmit.");
    return false;
}

auto RTC8583::EndTransmit() -> bool {
    if (state == State::TransmittingDataEnd) {
        ++address;
        state = State::TransmittingDataStart;
        return true;
    }
    spdlog::debug("RTC8583 unexpected end transmit.");
    return false;
}

}