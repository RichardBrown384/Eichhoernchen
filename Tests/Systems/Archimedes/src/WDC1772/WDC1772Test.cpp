#include "Archimedes/WDC1772Test.h"

#include "Common/Util/BitUtil.h"

constexpr auto CONTROL_REGISTER = 0u;
constexpr auto STATUS_REGISTER = 0u;
constexpr auto TRACK_REGISTER = 1u;
constexpr auto SECTOR_REGISTER = 2u;
constexpr auto DATA_REGISTER = 3u;

constexpr auto EncodeOptions(const TypeOneOptions& options) {
    return EncodeBitField(options.updateTrackRegister, 4u, 1u) +
           EncodeBitField(!options.motorOn, 3u, 1u) +
           EncodeBitField(options.verify, 2u, 1u) +
           EncodeBitField(options.stepRate, 0u, 2u);
}

constexpr auto EncodeOptions(const TypeTwoOptions& options) {
    return EncodeBitField(options.multipleSectors, 4u, 1u) +
        EncodeBitField(!options.motorOn, 3u, 1u) +
        EncodeBitField(options.settlingDelay, 2u, 1u) +
        EncodeBitField(options.deleteMark, 0u, 1u);
}

constexpr auto EncodeOptions(const TypeThreeOptions& options) {
    return EncodeBitField(!options.motorOn, 3u, 1u) +
           EncodeBitField(options.settlingDelay, 2u, 1u);
}

constexpr auto EncodeSeek(uint8_t type, const TypeOneOptions& options) {
    return EncodeBitField(type, 4u, 4u) + EncodeBitField(EncodeOptions(options), 0u, 4u);
}
constexpr auto EncodeStep(uint8_t type, const TypeOneOptions& options) {
    return EncodeBitField(type & ~1u, 4u, 4u) + EncodeBitField(EncodeOptions(options), 0u, 5u);
}
constexpr auto EncodeSector(uint8_t type, const TypeTwoOptions& options) {
    return EncodeBitField(type & ~1u, 4u, 4u) + EncodeBitField(EncodeOptions(options), 0u, 5u);
}
constexpr auto EncodeTrack(uint8_t type, const TypeThreeOptions& options) {
    return EncodeBitField(type, 4u, 4u) + EncodeBitField(EncodeOptions(options), 0u, 4u);
}

WDC1772Test::WDC1772Test() : mediator{}, disc{}, wdc1772{ mediator, disc } {}

auto WDC1772Test::Update(uint32_t time) -> void { wdc1772.Update(time); }

auto WDC1772Test::ReadRegisterStatus() -> uint32_t { return wdc1772.ReadByteRegister(STATUS_REGISTER); }
auto WDC1772Test::ReadRegisterControl() -> uint32_t { return wdc1772.ReadRegisterControl(); }
auto WDC1772Test::ReadRegisterTrack() -> uint32_t { return wdc1772.ReadByteRegister(TRACK_REGISTER); }
auto WDC1772Test::ReadRegisterSector() -> uint32_t { return wdc1772.ReadByteRegister(SECTOR_REGISTER); }
auto WDC1772Test::ReadRegisterData() -> uint32_t { return wdc1772.ReadByteRegister(DATA_REGISTER); }

auto WDC1772Test::WriteRegisterStatus(uint8_t v) -> void { wdc1772.WriteRegisterStatus(v); }
auto WDC1772Test::WriteRegisterControl(uint8_t v) -> void { wdc1772.WriteByteRegister(CONTROL_REGISTER, v); }
auto WDC1772Test::WriteRegisterTrack(uint8_t v) -> void { wdc1772.WriteByteRegister(TRACK_REGISTER, v); }
auto WDC1772Test::WriteRegisterSector(uint8_t v) -> void { wdc1772.WriteByteRegister(SECTOR_REGISTER, v); }
auto WDC1772Test::WriteRegisterData(uint8_t v) -> void { wdc1772.WriteByteRegister(DATA_REGISTER, v); }

auto WDC1772Test::TestStatusSet(uint8_t v) -> bool { return (wdc1772.ReadRegisterStatus() & v) == v; }
auto WDC1772Test::TestStatusClear(uint8_t v) -> bool { return (wdc1772.ReadRegisterStatus() & v) == 0u; }

auto WDC1772Test::GetHeadPositionTrack() const -> uint32_t { return wdc1772.GetHeadPositionTrack(); }
auto WDC1772Test::GetHeadPositionSector() const -> uint32_t { return wdc1772.GetHeadPositionSector(); }
auto WDC1772Test::SetHeadPositionTrack(uint32_t position) -> void { wdc1772.SetHeadPositionTrack(position); }
auto WDC1772Test::SetHeadPositionSector(uint32_t position, uint32_t limit) -> void {
    wdc1772.SetHeadPositionSector(position, limit);
}

auto WDC1772Test::WriteCommandRestore(const TypeOneOptions& options) -> void {
    WriteRegisterControl(EncodeSeek(0u, options));
}
auto WDC1772Test::WriteCommandSeek(const TypeOneOptions& options) -> void {
    WriteRegisterControl(EncodeSeek(1u, options));
}
auto WDC1772Test::WriteCommandStep(const TypeOneOptions& options) -> void {
    WriteRegisterControl(EncodeStep(2u, options));
}
auto WDC1772Test::WriteCommandStepIn(const TypeOneOptions& options) -> void {
    WriteRegisterControl(EncodeStep(4u, options));
}
auto WDC1772Test::WriteCommandStepOut(const TypeOneOptions& options) -> void {
    WriteRegisterControl(EncodeStep(6u, options));
}
auto WDC1772Test::WriteCommandReadSector(const TypeTwoOptions& options) -> void {
    WriteRegisterControl(EncodeSector(8u, options));
}
auto WDC1772Test::WriteCommandWriteSector(const TypeTwoOptions& options) -> void {
    WriteRegisterControl(EncodeSector(10u, options));
}
auto WDC1772Test::WriteCommandReadAddress(const TypeThreeOptions& options) -> void {
    WriteRegisterControl(EncodeTrack(12u, options));
}
auto WDC1772Test::WriteCommandForceInterrupt(uint8_t v) -> void {
    WriteRegisterControl(EncodeBitField(13, 4u, 4u) + EncodeBitField(v, 0u, 4u));
}