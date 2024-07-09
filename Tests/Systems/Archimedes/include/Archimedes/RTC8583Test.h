#pragma once

#include "Archimedes/RTC8583.h"

#include <gtest/gtest.h>

constexpr auto RTC_WRITE_ADDRESS = 0xA0u;
constexpr auto RTC_READ_ADDRESS = 0xA1u;

class RTC8583Test : public testing::Test {
public:
    RTC8583Test();
    [[nodiscard]] auto ReadAddress() const -> uint8_t;
    [[nodiscard]] auto ReadRam(uint8_t) const -> uint8_t;
    auto WriteRam(uint8_t, uint8_t) -> void;
    auto Start() -> void;
    auto Stop() -> void;
    auto ReceiveTargetAddress(uint8_t) -> bool;
    auto ReceiveData(uint8_t) -> bool;
    auto StartTransmit(uint8_t&) -> bool;
    auto EndTransmit() -> bool;
private:
    rbrown::acorn::archimedes::RTC8583 m_rtc;
};