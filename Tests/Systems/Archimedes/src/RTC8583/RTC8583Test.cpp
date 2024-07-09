#include "Archimedes/RTC8583Test.h"

RTC8583Test::RTC8583Test() : m_rtc{} {}

auto RTC8583Test::ReadAddress() const -> uint8_t { return m_rtc.ReadAddress(); }
auto RTC8583Test::ReadRam(uint8_t a) const -> uint8_t { return m_rtc.ReadRam(a); }
auto RTC8583Test::WriteRam(uint8_t a, uint8_t v) -> void { return m_rtc.WriteRam(a, v); }

auto RTC8583Test::Start() -> void { m_rtc.Start(); }
auto RTC8583Test::Stop() -> void { m_rtc.Stop(); }

auto RTC8583Test::ReceiveTargetAddress(uint8_t address) -> bool {
    return m_rtc.ReceiveTargetAddress(address);
}
auto RTC8583Test::ReceiveData(uint8_t v) -> bool {
    return m_rtc.ReceiveData(v);
}
auto RTC8583Test::StartTransmit(uint8_t& v) -> bool {
    return m_rtc.StartTransmit(v);
}
auto RTC8583Test::EndTransmit() -> bool {
    return m_rtc.EndTransmit();
}