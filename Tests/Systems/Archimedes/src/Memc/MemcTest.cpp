#include "Archimedes/MemcTest.h"

#include "Common/Util/BitUtil.h"

constexpr std::initializer_list<uint8_t> LOW_ROM = {
    0x01u, 0x02u, 0x03u, 0x04u,
    0x05u, 0x06u, 0x07u, 0x08u,
    0x09u, 0x0Au, 0x0Bu, 0x0Cu,
};

constexpr std::initializer_list<uint8_t> HIGH_ROM = {
    0x11u, 0x12u, 0x13u, 0x14u,
    0x15u, 0x16u, 0x17u, 0x18u,
    0x19u, 0x1Au, 0x1Bu, 0x1Cu,
};

constexpr auto ADDRESS_OFFSET_PHYSICAL = 0x200'0000u;
constexpr auto ADDRESS_OFFSET_IO_CONTROLLER = 0x300'0000u;
constexpr auto ADDRESS_OFFSET_LOW_ROM = 0x340'0000u;
constexpr auto ADDRESS_OFFSET_HIGH_ROM = 0x380'0000u;

constexpr auto ADDRESS_OFFSET_VIDEO_CONTROLLER = 0x340'0000u;

// Address translation to address line mapping

constexpr auto MapPhysicalPageNumberToAddress4K(uint32_t pageNumber) -> uint32_t {
    // PPN6-PPN0 -> A6-A0
    return ExtractBitField(pageNumber, 0, 7u);
}

constexpr auto MapPhysicalPageNumberToAddress8K(uint32_t pageNumber) -> uint32_t {
    // PPN6 -> A0
    // PPN5-PPN0 -> A6-A1
    const auto a0 = ExtractBitField(pageNumber, 6u, 1u);
    const auto a1 = ExtractBitField(pageNumber, 0u, 6u);
    return EncodeBitField(a0, 0u, 1u) +
           EncodeBitField(a1, 1u, 6u);
}

constexpr auto MapPhysicalPageNumberToAddress16K(uint32_t pageNumber) -> uint32_t {
    // PPN6-PPN5 -> A1-A0
    // PPN4-PPN0 -> A6-A2
    const auto a0 = ExtractBitField(pageNumber, 5u, 2u);
    const auto a2 = ExtractBitField(pageNumber, 0u, 5u);
    return EncodeBitField(a0, 0u, 2u) +
           EncodeBitField(a2, 2u, 5u);
}

constexpr auto MapPhysicalPageNumberToAddress32K(uint32_t pageNumber) -> uint32_t {
    // PPN4 -> A0
    // PPN6 -> A1
    // PPN5 -> A2
    // PPN3-PPN0 -> A6-A3
    const auto a0 = ExtractBitField(pageNumber, 4u, 1u);
    const auto a1 = ExtractBitField(pageNumber, 6u, 1u);
    const auto a2 = ExtractBitField(pageNumber, 5u, 1u);
    const auto a3 = ExtractBitField(pageNumber, 0u, 4u);
    return EncodeBitField(a0, 0u, 1u) +
           EncodeBitField(a1, 1u, 1u) +
           EncodeBitField(a2, 2u, 1u) +
           EncodeBitField(a3, 3u, 4u);
}

constexpr auto MapPhysicalPageNumberToAddress(uint32_t pageSize, uint32_t pageNumber) -> uint32_t {
    switch (pageSize) {
        case 0u: return MapPhysicalPageNumberToAddress4K(pageNumber);
        case 1u: return MapPhysicalPageNumberToAddress8K(pageNumber);
        case 2u: return MapPhysicalPageNumberToAddress16K(pageNumber);
        case 3u: return MapPhysicalPageNumberToAddress32K(pageNumber);
            [[unlikely]] default:return 0u;
    }
}

constexpr auto MapPageProtectionLevelToAddress(uint32_t pageProtectionLevel) -> uint32_t {
    return EncodeBitField(pageProtectionLevel, 8u, 2u);
}

constexpr auto MapLogicalPageNumberToAddress(uint32_t pageSize, uint32_t pageNumber) -> uint32_t {
    // Regardless of page size the upper bits of the page number are always mapped into a11-a10
    // So we simply shift up the larger page sizes into position
    // LPN11-LPN12 -> A11-A10
    // LPN10-LPN0 -> A22-A12
    const auto pageSizeBits = 13u - pageSize;
    const auto pageNumberForSize = ExtractBitField(pageNumber, 0u, pageSizeBits);
    const auto pageNumberShifted = EncodeBitField(pageNumberForSize, pageSize, pageSizeBits);
    const auto a10 = ExtractBitField(pageNumberShifted, 11u, 2u);
    const auto a12 = ExtractBitField(pageNumberShifted, 0u, 11u);
    return EncodeBitField(a10, 10u, 2u) +
           EncodeBitField(a12, 12u, 11u);
}

constexpr auto MapAddressTranslationToAddress(
    uint32_t pageSize,
    uint32_t physicalPageNumber,
    uint32_t logicalPageNumber,
    uint32_t pageProtectionLevel) -> uint32_t {
    return EncodeBitField(7u, 23, 3u) +
           MapLogicalPageNumberToAddress(pageSize, logicalPageNumber) +
           MapPageProtectionLevelToAddress(pageProtectionLevel) +
           MapPhysicalPageNumberToAddress(pageSize, physicalPageNumber);
}

constexpr auto MapAddressTranslationToAddress(
    const uint32_t pageSize,
    const AddressTranslation& translation) -> uint32_t {
    const auto&[physicalPageNumber, logicalPageNumber, pageProtectionLevel] = translation;
    return MapAddressTranslationToAddress(
        pageSize, physicalPageNumber, logicalPageNumber, pageProtectionLevel);
}

// Control word to address line mapping

constexpr auto MapControlWordToAddress(const ControlWord& word) -> uint32_t {
    return EncodeBitField(0x36u, 20u, 6u) +
           EncodeBitField(7u, 17, 4u) +
           EncodeBitField(word.operatingSystemMode, 12u, 1u) +
           EncodeBitField(word.soundDmaControl, 11u, 1u) +
           EncodeBitField(word.videoDmaControl, 10u, 1u) +
           EncodeBitField(word.dramRefreshControl, 8u, 2u) +
           EncodeBitField(word.highRomAccessTime, 6u, 2u) +
           EncodeBitField(word.lowRomAccessTime, 4u, 2u) +
           EncodeBitField(word.logicalPageSize, 2u, 2u);
}

// Dma address generators to address line mapping
constexpr auto MapDmaAddressGeneratorWordToAddress(const DmaAddressGeneratorWord& word) -> uint32_t {
    return EncodeBitField(0x36u, 20u, 6u) +
           EncodeBitField(word.reg, 17u, 3u) +
           EncodeBitField(word.value, 2u, 15u);
}

MemcTest::MemcTest(uint32_t ramSize) : mediator{}, memc(mediator, ramSize, LOW_ROM, HIGH_ROM) {}

void MemcTest::SetUp() { SetRomContinuouslyEnabled(false); CycleS(); }

auto MemcTest::CycleS() -> void { memc.CycleS(); }
auto MemcTest::CycleN() -> void { memc.CycleN(); }
auto MemcTest::CycleI(uint32_t c) -> void { memc.CycleI(c); }

// Enable triggering of reset and other events
auto MemcTest::Reset() -> void { memc.Reset(); }
auto MemcTest::EndFlyBack() -> void { memc.EndFlyBack(); }
auto MemcTest::StartVideoDmaRequest() -> void { memc.StartVideoDmaRequest(); }
auto MemcTest::StartCursorDmaRequest() -> void { memc.StartCursorDmaRequest(); }
auto MemcTest::StartSoundDmaRequest() -> void { memc.StartSoundDmaRequest(); }
auto MemcTest::EndVideoDmaRequest() -> void { memc.EndVideoDmaRequest(); }
auto MemcTest::EndCursorDmaRequest() -> void { memc.EndCursorDmaRequest(); }
auto MemcTest::EndSoundDmaRequest() -> void { memc.EndSoundDmaRequest(); }

// Write access to control state
auto MemcTest::SetSupervisor(bool v) -> void { memc.SetSupervisor(v); }
auto MemcTest::SetRomContinuouslyEnabled(bool v) -> void { memc.SetRomContinuouslyEnabled(v); }

// Read access to control state
auto MemcTest::GetSupervisor() const -> bool { return memc.GetSupervisor(); }
auto MemcTest::GetRomContinuouslyEnabled() const -> bool { return memc.GetRomContinuouslyEnabled();}

auto MemcTest::GetOperatingSystemMode() const -> bool { return memc.GetOperatingSystemMode(); }
auto MemcTest::GetSoundDmaControl() const -> bool { return memc.GetSoundDmaControl(); }
auto MemcTest::GetVideoDmaControl() const -> bool { return memc.GetVideoDmaControl(); }
auto MemcTest::GetDramRefreshControl() const -> uint32_t { return memc.GetDramRefreshControl(); }
auto MemcTest::GetHighRomAccessTime() const -> uint32_t { return memc.GetHighRomAccessTime(); }
auto MemcTest::GetLowRomAccessTime() const -> uint32_t { return memc.GetLowRomAccessTime(); }
auto MemcTest::GetPageSize() const -> uint32_t { return memc.GetPageSize(); }

auto MemcTest::GetVideoInit() const -> uint32_t { return memc.GetVideoInit(); }
auto MemcTest::GetVideoStart() const -> uint32_t { return memc.GetVideoStart(); }
auto MemcTest::GetVideoEnd() const -> uint32_t { return memc.GetVideoEnd(); }
auto MemcTest::GetCursorInit() const -> uint32_t { return memc.GetCursorInit(); }
auto MemcTest::GetSoundStart() const -> uint32_t { return memc.GetSoundStart(); }
auto MemcTest::GetSoundEndNext() const -> uint32_t { return memc.GetSoundEndNext(); }
auto MemcTest::GetSoundEndCurrent() const -> uint32_t { return memc.GetSoundEndCurrent(); }
auto MemcTest::GetNextSoundBufferValid() const -> bool { return memc.GetNextSoundBufferValid(); }

auto MemcTest::GetVideoPointer() const -> uint32_t { return memc.GetVideoPointer(); }
auto MemcTest::GetCursorPointer() const -> uint32_t { return memc.GetCursorPointer(); }
auto MemcTest::GetSoundPointer() const -> uint32_t { return memc.GetSoundPointer(); }

auto MemcTest::GetVideoDmaRequests() const -> uint32_t { return memc.GetVideoDmaRequests(); }
auto MemcTest::GetCursorDmaRequests() const -> uint32_t { return memc.GetCursorDmaRequests(); }
auto MemcTest::GetSoundDmaRequests() const -> uint32_t { return memc.GetSoundDmaRequests(); }

// Word reads
auto MemcTest::ReadWordLogical(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadWord(address, v);
}
auto MemcTest::ReadWordLogical(uint32_t page, uint32_t offset, uint32_t& v) -> bool {
    return ReadWordLogical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::ReadWordPhysical(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadWord(address + ADDRESS_OFFSET_PHYSICAL, v);
}
auto MemcTest::ReadWordPhysical(uint32_t page, uint32_t offset, uint32_t& v) -> bool {
    return ReadWordPhysical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::ReadWordInputOutputController(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadWord(address + ADDRESS_OFFSET_IO_CONTROLLER, v);
}
auto MemcTest::ReadWordLowRom(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadWord(address + ADDRESS_OFFSET_LOW_ROM, v);
}
auto MemcTest::ReadWordHighRom(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadWord(address + ADDRESS_OFFSET_HIGH_ROM, v);
}

// Byte reads
auto MemcTest::ReadByteLogical(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadByte(address, v);
}
auto MemcTest::ReadByteLogical(uint32_t page, uint32_t offset, uint32_t& v) -> bool {
    return ReadByteLogical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::ReadBytePhysical(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadByte(address + ADDRESS_OFFSET_PHYSICAL, v);
}
auto MemcTest::ReadByteInputOutputController(uint32_t address, uint32_t& v) -> bool {
    return memc.ReadByte(address + ADDRESS_OFFSET_IO_CONTROLLER, v);
}
auto MemcTest::ReadByteLowRom(uint32_t address, uint32_t & v) -> bool {
    return memc.ReadByte(address + ADDRESS_OFFSET_LOW_ROM, v);
}
auto MemcTest::ReadByteHighRom(uint32_t address, uint32_t & v) -> bool {
    return memc.ReadByte(address + ADDRESS_OFFSET_HIGH_ROM, v);
}

// Word writes
auto MemcTest::WriteWordLogical(uint32_t address, uint32_t v) -> bool {
    return memc.WriteWord(address, v);
}
auto MemcTest::WriteWordLogical(uint32_t page, uint32_t offset, uint32_t v) -> bool {
    return WriteWordLogical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::WriteWordPhysical(uint32_t address, uint32_t v) -> bool {
    return memc.WriteWord(address + ADDRESS_OFFSET_PHYSICAL, v);
}
auto MemcTest::WriteWordPhysical(uint32_t page, uint32_t offset, uint32_t v) -> bool {
    return WriteWordPhysical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::WriteWordInputOutputController(uint32_t address, uint32_t v) -> bool {
    return memc.WriteWord(address + ADDRESS_OFFSET_IO_CONTROLLER, v);
}
auto MemcTest::WriteWordVideoController(uint32_t address, uint32_t v) -> bool {
    return memc.WriteWord(address + ADDRESS_OFFSET_VIDEO_CONTROLLER, v);
}
auto MemcTest::WriteWordMemcControl(const ControlWord& word) -> bool {
    return memc.WriteWord(MapControlWordToAddress(word), 0u);
}
auto MemcTest::WriteWordDmaAddressGenerator(const DmaAddressGeneratorWord& word) -> bool {
    return memc.WriteWord(MapDmaAddressGeneratorWordToAddress(word), 0u);
}
auto MemcTest::WriteWordAddressTranslator(const AddressTranslation& translation) -> bool {
    return memc.WriteWord(MapAddressTranslationToAddress(memc.GetPageSize(), translation), 0u);
}

// Byte writes
auto MemcTest::WriteByteLogical(uint32_t address, uint32_t v) -> bool {
    return memc.WriteByte(address, v);
}
auto MemcTest::WriteByteLogical(uint32_t page, uint32_t offset, uint32_t v) -> bool {
    return WriteByteLogical(page * memc.GetPageSizeBytes() + offset, v);
}
auto MemcTest::WriteBytePhysical(uint32_t address, uint32_t v) -> bool {
    return memc.WriteByte(address + ADDRESS_OFFSET_PHYSICAL, v);
}
auto MemcTest::WriteByteInputOutputController(uint32_t address, uint32_t v) -> bool {
    return memc.WriteByte(address + ADDRESS_OFFSET_IO_CONTROLLER, ReplicateByteInWord(v));
}
auto MemcTest::WriteByteVideoController(uint32_t address, uint32_t v) -> bool {
    return memc.WriteByte(address + ADDRESS_OFFSET_VIDEO_CONTROLLER, ReplicateByteInWord(v));
}
auto MemcTest::WriteByteMemcControl(const ControlWord& word) -> bool {
    return memc.WriteByte(MapControlWordToAddress(word), 0u);
}
auto MemcTest::WriteByteDmaAddressGenerator(const DmaAddressGeneratorWord& word) -> bool {
    return memc.WriteByte(MapDmaAddressGeneratorWordToAddress(word), 0u);
}
auto MemcTest::WriteByteAddressTranslator(const AddressTranslation& translation) -> bool {
    return memc.WriteByte(MapAddressTranslationToAddress(memc.GetPageSize(), translation), 0u);
}
