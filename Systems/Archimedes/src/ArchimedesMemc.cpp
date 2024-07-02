/*
 * The approach here is to always have 8192 logical pages regardless of
 * the actual operating mode.
 *
 * When there are 8192 4KB logical pages then each entry in the physicals address table is used
 * When there are 4096 8Kb logical pages then two entries in the physicals address table are used
 * per logical page mapping:
 * The first entry is used to map to the lower 4K of the physical page
 * The second entry is used to map to the upper 4K of the physical page
 * When there are 2048 16Kb logical pages then four entries in the physicals address table are used
 * When there are 1024 32kb logical pages then eight entries in the physicals address table are used
 *
 * When mapping from a logical page to a physical one we need to 3 properties
 *
 * 1. The physical address, limited to 16Mb (24 bits) (0 to 23)
 * 2. The page protection level (2 bits) (24 to 25)
 * 3. Has the entry actually been mapped? (1 bit) (26)
 *
 * These properties are stored in a 32-bit unsigned int which we call a physical page descriptor.
 * When a physical page descriptor is zero then the descriptor is invalid (as a consequence of bit 26)
 *
 * The physicals vector stores the mapping from the logical pages to the physical page descriptors. A zero
 * physical page descriptor means that the logical page requested isn't mapped to a physical page.
 *
 * The logicals vector stores the mapping from the physical pages to the logical ones. We need to store 3
 * properties
 *
 * 1. The logical page number (16 bits) (0 to 15)
 * 2. The page protection level (2 bits) (16 to 17)
 * 3. Has the entry actually been mapped (1 bit) (18)
 *
 * These properties are stored in a 32-bit int which we call a logical page descriptor.
 * When a logical page descriptor is zero then the descriptor is invalid (as a consequence of bit 18)
 *
 * Problems with this approach
 *
 * 1. We assume on reset that nothing is mapped. The VLSI manual says the address translator is undefined
 * on power up.
 * 2. We should note that the translator is not affected by reset, but they are effectively
 * scrambled on a page size change. (The VLSI manual doesn't say how, and we're currently not implementing this).
 * I presume the MEMC stores all the significant bits written on the address bus in its CAM and not just those are
 * relevant to the current logical page size.
 * 3. We're handling the situation where a logical page is mapped to two (or more) physical pages
 * by using a 'last mapped wins' approach. When a logical page is being unmapped we then search the
 * logicals table for a suitable replacement and map that page in. If no replacement is found then
 * we unmap the page completely. NB The VLSI manual says "when that logical page
 * is accessed, many entries will claim to match, and an invalid physical page number will result". It doesn't
 * actually specify what happens. Are the entries logically ORed or ANDed together? Does the address translation
 * cause an abort? This case does happen in practice.
 * 4. Writing to the translator does potentially too much work depending on page size.
 *
 */

#include "Archimedes/ArchimedesMemc.h"
#include "Archimedes/ArchimedesMediator.h"

#include "Common/Util/Shifts.h"
#include "Common/Util/BitUtil.h"

#include <array>

using namespace rbrown::acorn::archimedes;

constexpr auto CYCLE_RAM_N_NS = 250u;
constexpr auto CYCLE_RAM_S_NS = 125u;
constexpr auto CYCLE_I_NS = 125u;

constexpr auto CYCLE_IOC_NS = 250u;
constexpr auto CYCLE_ROM_450_NS = 450u;
constexpr auto CYCLE_ROM_325_NS = 325u;
constexpr auto CYCLE_ROM_200_NS = 200u;
constexpr std::array CYCLE_ROM_NS = {
    CYCLE_ROM_450_NS, CYCLE_ROM_325_NS, CYCLE_ROM_200_NS, CYCLE_ROM_200_NS
};
constexpr auto CYCLE_VIDC_NS = 125u;
constexpr auto CYCLE_MEMC_CONTROL_NS = 125u;
constexpr auto CYCLE_ADDRESS_TRANSLATOR_NS = 125u;

constexpr auto ADDRESS_MASK_26_BIT = 0x3FF'FFFFu;
constexpr auto ADDRESS_OFFSET_PHYSICAL = 0x200'0000u;
constexpr auto ADDRESS_OFFSET_LOW_ROM = 0x340'0000u;
constexpr auto ADDRESS_OFFSET_HIGH_ROM = 0x380'0000u;

constexpr auto ADDRESS_MASK_DMA = 0x7FFFu;
constexpr auto ADDRESS_MASK_A3A2 = 0xCu;

constexpr std::array MAX_LEVELS_READ = { 1u, 3u, 3u };
constexpr std::array MAX_LEVELS_WRITE = { 0u, 1u, 3u };

constexpr auto DMA_REQUEST_LIMIT = 300u;

// Address line decoding
constexpr auto MapAddressToPhysicalPageNumber4K(uint32_t address) -> uint32_t {
    // A6-A0 -> PPN6-PPN0
    return ExtractBitField(address, 0u, 7u);
}

constexpr auto MapAddressToPhysicalPageNumber8K(uint32_t address) -> uint32_t {
    // A6-A1 -> PPN5-PPN0
    // A0 -> PPN6
    const auto ppn0 = ExtractBitField(address, 1u, 6u);
    const auto ppn6 = ExtractBitField(address, 0u, 1u);
    return EncodeBitField(ppn0, 0u, 6u) +
           EncodeBitField(ppn6, 6u, 1u);
}

constexpr auto MapAddressToPhysicalPageNumber16K(uint32_t address) -> uint32_t {
    // A6-A2 -> PPN4-PPN0
    // A1-A0 -> PPN6-PPN5
    const auto ppn0 = ExtractBitField(address, 2u, 5u);
    const auto ppn5 = ExtractBitField(address, 0u, 2u);
    return EncodeBitField(ppn0, 0u, 5u) +
           EncodeBitField(ppn5, 5u, 2u);
}

constexpr auto MapAddressToPhysicalPageNumber32K(uint32_t address) -> uint32_t {
    // A6-A3 -> PPN3-PPN0
    // A0 -> PPN4
    // A2 -> PPN5
    // A1 -> PPN6
    const auto ppn0 = ExtractBitField(address, 3u, 4u);
    const auto ppn4 = ExtractBitField(address, 0u, 1u);
    const auto ppn5 = ExtractBitField(address, 2u, 1u);
    const auto ppn6 = ExtractBitField(address, 1u, 1u);
    return EncodeBitField(ppn0, 0u, 4u) +
           EncodeBitField(ppn4, 4u, 1u) +
           EncodeBitField(ppn5, 5u, 1u) +
           EncodeBitField(ppn6, 6u, 1u);
}

constexpr auto MapAddressToPhysicalPageNumber(uint32_t pageSize, uint32_t address) -> uint32_t {
    switch (pageSize) {
        case 0u: return MapAddressToPhysicalPageNumber4K(address);
        case 1u: return MapAddressToPhysicalPageNumber8K(address);
        case 2u: return MapAddressToPhysicalPageNumber16K(address);
        case 3u: return MapAddressToPhysicalPageNumber32K(address);
            [[unlikely]] default:return 0u;
    }
}

constexpr auto MapAddressToPageProtectionLevel(uint32_t address) -> uint32_t {
    // A9-A8 -> PPL1-PPL0
    return ExtractBitField(address, 8u, 2u);
}

constexpr auto MapAddressToLogicalPageNumber(uint32_t pageSize, uint32_t address) -> uint32_t {
    // A11-A10 -> LPN12-LPN11
    // A22-A12 -> LPN10-LPN0
    return (LogicalShiftLeft(ExtractBitField(address, 10u, 2u), 11u) +
            ExtractBitField(address, 12u, 11u)) >> pageSize;
}

// Physical Page descriptors
constexpr auto EncodePhysicalPageDescriptor(uint32_t physicalAddress, uint32_t pageProtectionLevel) -> uint32_t {
    return EncodeBitField(physicalAddress, 0u, 24u) +
           EncodeBitField(pageProtectionLevel, 24u, 2u) +
           EncodeBitField(1u, 26u, 1u);
}

constexpr auto ExtractPhysicalPageDescriptorAddress(uint32_t v) -> uint32_t {
    return ExtractBitField(v, 0u, 24u);
}

constexpr auto ExtractPhysicalPageDescriptorPageProtectionLevel(uint32_t v) -> uint32_t {
    return ExtractBitField(v, 24u, 2u);
}

// Logical Page Descriptors
constexpr auto EncodeLogicalPageDescriptor(uint32_t logicalPageNumber, uint32_t pageProtectionLevel) -> uint32_t {
    return EncodeBitField(logicalPageNumber, 0u, 16u) +
           EncodeBitField(pageProtectionLevel, 16u, 2u) +
           EncodeBitField(1u, 18u, 1u);
}

constexpr auto ExtractLogicalPageDescriptorLogicalPageNumber(uint32_t v) {
    return ExtractBitField(v, 0u, 16u);
}

constexpr auto ExtractLogicalPageDescriptorPageProtectionLevel(uint32_t v) {
    return ExtractBitField(v, 16u, 2u);
}

// Safe memory access functions
constexpr auto ReadByte(const std::vector<uint8_t>& v, uint32_t offset) {
    return v[offset % v.size()];
}

constexpr auto ReadWord(const std::vector<uint8_t>& v, uint32_t offset) {
    offset &= ~3u;
    const auto a = ReadByte(v, offset);
    const auto b = ReadByte(v, offset + 1u);
    const auto c = ReadByte(v, offset + 2u);
    const auto d = ReadByte(v, offset + 3u);
    return (d << 24u) + (c << 16u) + (b << 8u) + a;
}

constexpr auto WriteByte(std::vector<uint8_t>& v, uint32_t offset, uint32_t value) {
    v[offset % v.size()] = static_cast<uint8_t>(value);
}

constexpr auto WriteWord(std::vector<uint8_t>& v, uint32_t offset, uint32_t value) {
    offset &= ~3u;
    WriteByte(v, offset, value);
    WriteByte(v, offset + 1u, value >> 8u);
    WriteByte(v, offset + 2u, value >> 16u);
    WriteByte(v, offset + 3u, value >> 24u);
}

constexpr auto ExtractMemoryRegion(uint32_t address) -> uint32_t {
    return ExtractBitField(address, 20u, 6u);
}

constexpr auto MaskDmaAddress(uint32_t v) -> uint32_t {
    return v & ADDRESS_MASK_DMA;
}

constexpr auto IsAddressSequential(uint32_t address) -> bool {
    return address & ADDRESS_MASK_A3A2;
}

ArchimedesMemc::ArchimedesMemc(ArchimedesMediator& mediator, uint32_t ramSize, std::vector<uint8_t> low, std::vector<uint8_t> high) :
    supervisor{ true },
    operatingSystemMode{},
    soundDmaControl{},
    videoDmaControl{},
    dramRefreshControl{},
    highRomAccessTime{},
    lowRomAccessTime{},
    pageSize{},
    videoInit{},
    videoStart{},
    videoEnd{},
    cursorInit{},
    soundStart{},
    soundEndNext{},
    soundEndCurrent{},
    nextSoundBufferValid{},
    videoPointer{},
    cursorPointer{},
    soundPointer{},
    videoDmaRequests{},
    cursorDmaRequests{},
    soundDmaRequests{},
    sequential{},
    romContinuouslyEnabled{ true },
    lowRomAccessTimeNanoSeconds { CYCLE_ROM_450_NS },
    highRomAccessTimeNanoSeconds { CYCLE_ROM_450_NS },
    readProtectionLevel{ 3u },
    writeProtectionLevel{ 3u },
    logicals(128, 0u),
    physicals(8192, 0u),
    ram(ramSize, 0u),
    lowRom(std::move(low)),
    highRom(std::move(high)),
    mediator{mediator} {}

auto ArchimedesMemc::GetSupervisor() const -> bool { return supervisor; }
auto ArchimedesMemc::GetRomContinuouslyEnabled() const -> bool {  return romContinuouslyEnabled; }

auto ArchimedesMemc::GetOperatingSystemMode() const -> bool { return operatingSystemMode; }
auto ArchimedesMemc::GetSoundDmaControl() const -> bool { return soundDmaControl; }
auto ArchimedesMemc::GetVideoDmaControl() const -> bool { return videoDmaControl; }
auto ArchimedesMemc::GetDramRefreshControl() const -> uint32_t { return dramRefreshControl; }
auto ArchimedesMemc::GetHighRomAccessTime() const -> uint32_t { return highRomAccessTime; }
auto ArchimedesMemc::GetLowRomAccessTime() const -> uint32_t { return lowRomAccessTime; }
auto ArchimedesMemc::GetPageSize() const -> uint32_t { return pageSize; }
auto ArchimedesMemc::GetPageSizeBytes() const -> uint32_t { return LogicalShiftLeft(1u, 12u + pageSize); }

auto ArchimedesMemc::GetSequential() const -> bool { return sequential; }

auto ArchimedesMemc::GetVideoInit() const -> uint32_t { return videoInit; }
auto ArchimedesMemc::GetVideoStart() const -> uint32_t { return videoStart; }
auto ArchimedesMemc::GetVideoEnd() const -> uint32_t { return videoEnd; }
auto ArchimedesMemc::GetCursorInit() const -> uint32_t { return cursorInit; }
auto ArchimedesMemc::GetSoundStart() const -> uint32_t { return soundStart; }
auto ArchimedesMemc::GetSoundEndNext() const -> uint32_t { return soundEndNext; }
auto ArchimedesMemc::GetSoundEndCurrent() const -> uint32_t { return soundEndCurrent; }
auto ArchimedesMemc::GetNextSoundBufferValid() const -> bool { return nextSoundBufferValid; }

auto ArchimedesMemc::GetVideoPointer() const -> uint32_t { return videoPointer; }
auto ArchimedesMemc::GetCursorPointer() const -> uint32_t { return cursorPointer; }
auto ArchimedesMemc::GetSoundPointer() const -> uint32_t { return soundPointer; }

auto ArchimedesMemc::GetVideoDmaRequests() const -> uint32_t { return videoDmaRequests; }
auto ArchimedesMemc::GetCursorDmaRequests() const -> uint32_t { return cursorDmaRequests; }
auto ArchimedesMemc::GetSoundDmaRequests() const -> uint32_t { return soundDmaRequests; }
auto ArchimedesMemc::GetOutstandingDmaRequests() const -> bool {
    return GetVideoDmaRequests() || GetCursorDmaRequests() || GetSoundDmaRequests();
}

auto ArchimedesMemc::SetSupervisor(bool v) -> void {
    supervisor = v;
    const auto level = supervisor ? 2u : operatingSystemMode ? 1u : 0u;
    readProtectionLevel = MAX_LEVELS_READ[level];
    writeProtectionLevel = MAX_LEVELS_WRITE[level];
}
auto ArchimedesMemc::SetRomContinuouslyEnabled(bool v) -> void { romContinuouslyEnabled = v; }

auto ArchimedesMemc::SetOperatingSystemMode(bool v) -> void { operatingSystemMode = v; }
auto ArchimedesMemc::SetSoundDmaControl(bool v) -> void {
    if (!v) { SetSoundDmaRequests(0u); }
    soundDmaControl = v;
}
auto ArchimedesMemc::SetVideoDmaControl(bool v) -> void {
    if (!v) {
        SetVideoDmaRequests(0u);
        SetCursorDmaRequests(0u);
    }
    videoDmaControl = v;
}
auto ArchimedesMemc::SetDramRefreshControl(uint32_t v) -> void { dramRefreshControl = v & 3u; }
auto ArchimedesMemc::SetHighRomAccessTime(uint32_t v) -> void {
    highRomAccessTime = v & 3u;
    highRomAccessTimeNanoSeconds = CYCLE_ROM_NS[highRomAccessTime];
}
auto ArchimedesMemc::SetLowRomAccessTime(uint32_t v) -> void {
    lowRomAccessTime = v & 3u;
    lowRomAccessTimeNanoSeconds = CYCLE_ROM_NS[lowRomAccessTime];
}
auto ArchimedesMemc::SetPageSize(uint32_t v) -> void { pageSize = v & 3u; }

auto ArchimedesMemc::SetSequential(bool v) -> void { sequential = v; }

auto ArchimedesMemc::SetVideoInit(uint32_t v) -> void { videoInit = MaskDmaAddress(v); }
auto ArchimedesMemc::SetVideoStart(uint32_t v) -> void { videoStart = MaskDmaAddress(v); }
auto ArchimedesMemc::SetVideoEnd(uint32_t v) -> void { videoEnd = MaskDmaAddress(v); }
auto ArchimedesMemc::SetCursorInit(uint32_t v) -> void { cursorInit = MaskDmaAddress(v); }
auto ArchimedesMemc::SetSoundStart(uint32_t v) -> void {
    soundStart = MaskDmaAddress(v);
    SetNextSoundBufferValid(true);
    mediator.EndSoundInterrupt();
}
auto ArchimedesMemc::SetSoundEndNext(uint32_t v) -> void { soundEndNext = MaskDmaAddress(v); }
auto ArchimedesMemc::SetSoundPointer() -> void {
    if (GetNextSoundBufferValid()) {
        std::swap(soundEndCurrent, soundEndNext);
        SetNextSoundBufferValid(false);
    }
    SetSoundPointer(GetSoundStart());
    mediator.StartSoundInterrupt();
}
auto ArchimedesMemc::SetNextSoundBufferValid(bool v) -> void { nextSoundBufferValid = v; }

auto ArchimedesMemc::SetVideoPointer(uint32_t v) -> void { videoPointer = v; }
auto ArchimedesMemc::SetCursorPointer(uint32_t v) -> void { cursorPointer = v; }
auto ArchimedesMemc::SetSoundPointer(uint32_t v) -> void { soundPointer = v; }

auto ArchimedesMemc::SetVideoDmaRequests(uint32_t v) -> void { videoDmaRequests = v; }
auto ArchimedesMemc::SetCursorDmaRequests(uint32_t v) -> void { cursorDmaRequests = v; }
auto ArchimedesMemc::SetSoundDmaRequests(uint32_t v) -> void { soundDmaRequests = v; }

auto ArchimedesMemc::Reset() -> void {
    SetSupervisor(true);
    SetOperatingSystemMode(false);
    SetSoundDmaControl(false);
    SetHighRomAccessTime(0u);
    SetLowRomAccessTime(0u);
    SetPageSize(0u);
    SetRomContinuouslyEnabled(true);
    SetNextSoundBufferValid(false);
    mediator.StartSoundInterrupt();
    SetVideoDmaRequests(0u);
    SetCursorDmaRequests(0u);
    SetSoundDmaRequests(0u);
}

auto ArchimedesMemc::EndFlyBack() -> void {
    SetVideoPointer(GetVideoInit());
    SetCursorPointer(GetCursorInit());
}
auto ArchimedesMemc::StartVideoDmaRequest() -> void { if (GetVideoDmaControl()) { ++videoDmaRequests; }}
auto ArchimedesMemc::StartCursorDmaRequest() -> void { if (GetVideoDmaControl()) { ++cursorDmaRequests; }}
auto ArchimedesMemc::StartSoundDmaRequest() -> void { if (GetSoundDmaControl()) { ++soundDmaRequests; }}
auto ArchimedesMemc::EndVideoDmaRequest() -> void { if (videoDmaRequests) { --videoDmaRequests; }}
auto ArchimedesMemc::EndCursorDmaRequest() -> void { if (cursorDmaRequests) { --cursorDmaRequests; }}
auto ArchimedesMemc::EndSoundDmaRequest() -> void { if (soundDmaRequests) { --soundDmaRequests; }}

auto ArchimedesMemc::CycleS() -> void { SetSequential(true); }
auto ArchimedesMemc::CycleN() -> void { SetSequential(false); }
auto ArchimedesMemc::CycleI(uint32_t count) -> void {
    for (auto i = 0u; i < count; ++i) {
        UpdateDma();
        mediator.Update(CYCLE_I_NS);
    }
}

auto ArchimedesMemc::IsCycleSequential(uint32_t address) const -> bool {
    return GetSequential() && IsAddressSequential(address);
}

auto ArchimedesMemc::UpdateRamAccess(uint32_t address) -> void {
    mediator.Update(IsCycleSequential(address) ? CYCLE_RAM_S_NS : CYCLE_RAM_N_NS);
}
auto ArchimedesMemc::UpdateIoControl() -> void { mediator.Update(CYCLE_IOC_NS); }
auto ArchimedesMemc::UpdateLowRom() -> void { mediator.Update(lowRomAccessTimeNanoSeconds); }
auto ArchimedesMemc::UpdateHighRom() -> void { mediator.Update(highRomAccessTimeNanoSeconds); }
auto ArchimedesMemc::UpdateVidc() -> void { mediator.Update(CYCLE_VIDC_NS); }
auto ArchimedesMemc::UpdateMemcControl() -> void { mediator.Update(CYCLE_MEMC_CONTROL_NS); }
auto ArchimedesMemc::UpdateAddressTranslator() -> void { mediator.Update(CYCLE_ADDRESS_TRANSLATOR_NS); }

auto ArchimedesMemc::UpdateDma() -> void {
    // Note: it's possible to generate dma requests while serving 
    // dma requests. Therefore, there's a risk of an infinite loop.
    // We impose an arbitrary limit here in order to guarantee
    // that our callers can still process messages from the OS.
    for (auto i = 0u; i < DMA_REQUEST_LIMIT && GetOutstandingDmaRequests(); ++i) {
        if (GetCursorDmaRequests()) {
            UpdateCursorDma();
        } else if (GetVideoDmaRequests()) {
            UpdateVideoDma();
        } else if (GetSoundDmaRequests()) {
            UpdateSoundDma();
        }
    }
}

auto ArchimedesMemc::UpdateVideoDma() -> void {
    const auto pointer = GetVideoPointer();
    const auto end = GetVideoEnd();
    const auto start = GetVideoStart();
    const auto address = pointer << 4u;
    mediator.WriteWordVidcVideo(ReadWordDmaPhysicalN(address));
    mediator.WriteWordVidcVideo(ReadWordDmaPhysicalS(address + 4u));
    mediator.WriteWordVidcVideo(ReadWordDmaPhysicalS(address + 8u));
    mediator.WriteWordVidcVideo(ReadWordDmaPhysicalS(address + 12u));
    if (pointer < end) {
        SetVideoPointer(pointer + 1u);
    } else {
        SetVideoPointer(start);
    }
    EndVideoDmaRequest();
    mediator.EndVideoDmaRequest();
}

auto ArchimedesMemc::UpdateCursorDma() -> void {
    const auto pointer = GetCursorPointer();
    const auto address = pointer << 4u;
    mediator.WriteWordVidcCursor(ReadWordDmaPhysicalN(address));
    mediator.WriteWordVidcCursor(ReadWordDmaPhysicalS(address + 4u));
    mediator.WriteWordVidcCursor(ReadWordDmaPhysicalS(address + 8u));
    mediator.WriteWordVidcCursor(ReadWordDmaPhysicalS(address + 12u));
    SetCursorPointer(pointer + 1u);
    EndCursorDmaRequest();
    mediator.EndCursorDmaRequest();
}

auto ArchimedesMemc::UpdateSoundDma() -> void {
    const auto pointer = GetSoundPointer();
    const auto end = GetSoundEndCurrent();
    const auto address = pointer << 4u;
    mediator.WriteWordVidcSound(ReadWordDmaPhysicalN(address));
    mediator.WriteWordVidcSound(ReadWordDmaPhysicalS(address + 4u));
    mediator.WriteWordVidcSound(ReadWordDmaPhysicalS(address + 8u));
    mediator.WriteWordVidcSound(ReadWordDmaPhysicalS(address + 12u));
    if (pointer < end) {
        SetSoundPointer(pointer + 1u);
    } else {
        SetSoundPointer();
    }
    EndSoundDmaRequest();
    mediator.EndSoundDmaRequest();
}

auto ArchimedesMemc::ReadByte(uint32_t address, uint32_t& value) -> bool {
    if (!IsCycleSequential(address)) {
        UpdateDma();
    }
    address &= ADDRESS_MASK_26_BIT;
    const auto region = ExtractMemoryRegion(address);
    if (region < 0x20u) {
        return ReadByteLogical(address, value);
    } else if (region < 0x30u) {
        return ReadBytePhysical(address, value);
    } else if (region < 0x34u) {
        return ReadByteIoControl(address, value);
    } else if (region < 0x38u) {
        return ReadByteLowRom(address, value);
    } else {
        return ReadByteHighRom(address, value);
    }
}

auto ArchimedesMemc::ReadWord(uint32_t address, uint32_t& value) -> bool {
    if (!IsCycleSequential(address)) {
        UpdateDma();
    }
    if (GetRomContinuouslyEnabled()) [[unlikely]] {
        SetRomContinuouslyEnabled(!BitSet(address, 25u));
        if (GetRomContinuouslyEnabled()) {
            value = ::ReadWord(highRom, address);
            return true;
        }
    }
    address &= ADDRESS_MASK_26_BIT;
    const auto region = ExtractMemoryRegion(address);
    if (region < 0x20u) {
        return ReadWordLogical(address, value);
    } else if (region < 0x30u) {
        return ReadWordPhysical(address, value);
    } else if (region < 0x34u) {
        return ReadWordIoControl(address, value);
    } else if (region < 0x38u) {
        return ReadWordLowRom(address, value);
    } else {
        return ReadWordHighRom(address, value);
    }
}

auto ArchimedesMemc::WriteByte(uint32_t address, uint32_t value) -> bool {
    if (!IsCycleSequential(address)) {
        UpdateDma();
    }
    address &= ADDRESS_MASK_26_BIT;
    const auto region = ExtractMemoryRegion(address);
    if (region < 0x20u) {
        return WriteByteLogical(address, value);
    } else if (region < 0x30u) {
        return WriteBytePhysical(address, value);
    } else if (region < 0x34u) {
        return WriteByteIoControl(address, value);
    } else if (region < 0x36u) {
        return WriteByteVidc(value);
    } else if (region < 0x38u) {
        return WriteByteMemcControl(address);
    } else {
        return WriteByteAddressTranslator(address);
    }
}

auto ArchimedesMemc::WriteWord(uint32_t address, uint32_t value) -> bool {
    if (!IsCycleSequential(address)) {
        UpdateDma();
    }
    address &= ADDRESS_MASK_26_BIT;
    const auto region = ExtractMemoryRegion(address);
    if (region < 0x20u) {
        return WriteWordLogical(address, value);
    } else if (region < 0x30u) {
        return WriteWordPhysical(address, value);
    } else if (region < 0x34u) {
        return WriteWordIoControl(address, value);
    } else if (region < 0x36u) {
        return WriteWordVidc(value);
    } else if (region < 0x38u) {
        return WriteWordMemcControl(address);
    } else {
        return WriteWordAddressTranslator(address);
    }
}

// Byte reads
auto ArchimedesMemc::ReadByteLogical(uint32_t logicalAddress, uint32_t& value) -> bool {
    UpdateRamAccess(logicalAddress);
    uint32_t physicalAddress;
    if (!TranslateReadLogicalAddress(logicalAddress, physicalAddress)) {
        return false;
    }
    value = ::ReadByte(ram, physicalAddress);
    return true;
}

auto ArchimedesMemc::ReadBytePhysical(uint32_t address, uint32_t& value) -> bool {
    UpdateRamAccess(address);
    if (!GetSupervisor()) {
        return false;
    }
    value = ::ReadByte(ram, address - ADDRESS_OFFSET_PHYSICAL);
    return true;
}

auto ArchimedesMemc::ReadByteIoControl(uint32_t address, uint32_t& value) -> bool {
    if (ReadWordIoControl(address, value)) {
        value = ExtractBitField(value, 0u, 8u);
        return true;
    }
    return false;
}

auto ArchimedesMemc::ReadByteLowRom(uint32_t address, uint32_t& value) -> bool {
    UpdateLowRom();
    value = ::ReadByte(lowRom, address - ADDRESS_OFFSET_LOW_ROM);
    return true;
}

auto ArchimedesMemc::ReadByteHighRom(uint32_t address, uint32_t& value) -> bool {
    UpdateHighRom();
    value = ::ReadByte(highRom, address - ADDRESS_OFFSET_HIGH_ROM);
    return true;
}

// Word reads
auto ArchimedesMemc::ReadWordLogical(uint32_t logicalAddress, uint32_t& value) -> bool {
    UpdateRamAccess(logicalAddress);
    uint32_t physicalAddress;
    if (!TranslateReadLogicalAddress(logicalAddress, physicalAddress)) {
        return false;
    }
    value = ::ReadWord(ram, physicalAddress);
    return true;
}

auto ArchimedesMemc::ReadWordPhysical(uint32_t address, uint32_t& value) -> bool {
    UpdateRamAccess(address);
    if (!GetSupervisor()) {
        return false;
    }
    value = ::ReadWord(ram, address - ADDRESS_OFFSET_PHYSICAL);
    return true;
}

auto ArchimedesMemc::ReadWordIoControl(uint32_t address, uint32_t& value) -> bool {
    UpdateIoControl();
    if (!GetSupervisor()) {
        return false;
    }
    value = mediator.ReadWordIoc(address);
    return true;
}

auto ArchimedesMemc::ReadWordLowRom(uint32_t address, uint32_t& value) -> bool {
    UpdateLowRom();
    value = ::ReadWord(lowRom, address - ADDRESS_OFFSET_LOW_ROM);
    return true;
}

auto ArchimedesMemc::ReadWordHighRom(uint32_t address, uint32_t& value) -> bool {
    UpdateHighRom();
    value = ::ReadWord(highRom, address - ADDRESS_OFFSET_HIGH_ROM);
    return true;
}

auto ArchimedesMemc::ReadWordDmaPhysicalS(uint32_t address) -> uint32_t {
    mediator.Update(CYCLE_RAM_S_NS);
    return ::ReadWord(ram, address);
}

auto ArchimedesMemc::ReadWordDmaPhysicalN(uint32_t address) -> uint32_t {
    mediator.Update(CYCLE_RAM_N_NS);
    return ::ReadWord(ram, address);
}

// Byte writes
auto ArchimedesMemc::WriteByteLogical(uint32_t logicalAddress, uint32_t value) -> bool {
    UpdateRamAccess(logicalAddress);
    uint32_t physicalAddress;
    if (!TranslateWriteLogicalAddress(logicalAddress, physicalAddress)) {
        return false;
    }
    ::WriteByte(ram, physicalAddress, value);
    return true;
}

auto ArchimedesMemc::WriteBytePhysical(uint32_t address, uint32_t value) -> bool {
    UpdateRamAccess(address);
    if (!GetSupervisor()) {
        return false;
    }
    ::WriteByte(ram, address - ADDRESS_OFFSET_PHYSICAL, value);
    return true;
}

auto ArchimedesMemc::WriteByteIoControl(uint32_t address, uint32_t value) -> bool {
    return WriteWordIoControl(address, value);
}
auto ArchimedesMemc::WriteByteVidc(uint32_t value) -> bool { return WriteWordVidc(value); }
auto ArchimedesMemc::WriteByteMemcControl(uint32_t address) -> bool { return WriteWordMemcControl(address); }
auto ArchimedesMemc::WriteByteAddressTranslator(uint32_t address) -> bool { return WriteWordAddressTranslator(address); }

// Word writes
auto ArchimedesMemc::WriteWordLogical(uint32_t logicalAddress, uint32_t value) -> bool {
    UpdateRamAccess(logicalAddress);
    uint32_t physicalAddress;
    if (!TranslateWriteLogicalAddress(logicalAddress, physicalAddress)) {
        return false;
    }
    ::WriteWord(ram, physicalAddress, value);
    return true;
}

auto ArchimedesMemc::WriteWordPhysical(uint32_t address, uint32_t value) -> bool {
    UpdateRamAccess(address);
    if (!GetSupervisor()) {
        return false;
    }
    ::WriteWord(ram, address - ADDRESS_OFFSET_PHYSICAL, value);
    return true;
}

auto ArchimedesMemc::WriteWordIoControl(uint32_t address, uint32_t value) -> bool {
    UpdateIoControl();
    if (!GetSupervisor()) {
        return false;
    }
    mediator.WriteWordIoc(address, value);
    return true;
}

auto ArchimedesMemc::WriteWordVidc(uint32_t value) -> bool {
    UpdateVidc();
    if (!GetSupervisor()) {
        return false;
    }
    mediator.WriteWordVidcRegister(value);
    return true;
}

auto ArchimedesMemc::WriteWordMemcControl(uint32_t address) -> bool {
    UpdateMemcControl();
    if (!GetSupervisor()) {
        return false;
    }
    const auto value = ExtractBitField(address, 2u, 15u);
    switch (ExtractBitField(address, 17u, 3u)) {
        case 0u: SetVideoInit(value); return true;
        case 1u: SetVideoStart(value); return true;
        case 2u: SetVideoEnd(value); return true;
        case 3u: SetCursorInit(value); return true;
        case 4u: SetSoundStart(value); return true;
        case 5u: SetSoundEndNext(value); return true;
        case 6u: SetSoundPointer(); return true;
        case 7u: {
            SetOperatingSystemMode(BitSet(address, 12u));
            SetSoundDmaControl(BitSet(address, 11u));
            SetVideoDmaControl(BitSet(address, 10u));
            SetDramRefreshControl(ExtractBitField(address, 8u, 2u));
            SetHighRomAccessTime(ExtractBitField(address, 6u, 2u));
            SetLowRomAccessTime(ExtractBitField(address, 4u, 2u));
            SetPageSize(ExtractBitField(address, 2u, 2u));
            return true;
        }
        [[unlikely]] default: return false;
    }
}

auto ArchimedesMemc::WriteWordAddressTranslator(uint32_t address) -> bool {
    UpdateAddressTranslator();
    if (!GetSupervisor()) {
        return false;
    }

    // Decode the address
    const auto physicalPageNumber = MapAddressToPhysicalPageNumber(GetPageSize(), address);
    const auto logicalPageNumber = MapAddressToLogicalPageNumber(GetPageSize(), address);
    const auto pageProtectionLevel = MapAddressToPageProtectionLevel(address);

    // In the event that a logical page is mapped to more than one physical page
    // we need to scan the logicals table and find a replacement physical page and map that
    // if no replacement is found then we unmap the associated logical page.
    // This is to deal with the case where physicals[6] = 1000 and physicals[7] = 1000
    // and the caller wants to map logical page 999 to physical page 7. If we were to just
    // unmap logical page 1000 and the processor were then to access logical page 1000 expecting
    // to read from physical page 6 we'd then wrongly trigger a data abort.
    const auto oldLogicalPageDescriptor = logicals[physicalPageNumber];
    if (oldLogicalPageDescriptor) {
        const auto oldLogicalPageNumber = ExtractLogicalPageDescriptorLogicalPageNumber(oldLogicalPageDescriptor);
        const auto replacementPhysicalPageNumber = FindReplacementPhysicalPage(physicalPageNumber, oldLogicalPageNumber);
        if (replacementPhysicalPageNumber) {
            const auto descriptor = logicals[*replacementPhysicalPageNumber];
            const auto replacementPageProtectionLevel = ExtractLogicalPageDescriptorPageProtectionLevel(descriptor);
            WriteAddressTranslator(*replacementPhysicalPageNumber, oldLogicalPageNumber, replacementPageProtectionLevel);
        } else {
            ClearAddressTranslator(oldLogicalPageNumber);
        }
    }

    // Work out the starting physical address 
    WriteAddressTranslator(physicalPageNumber, logicalPageNumber, pageProtectionLevel);

    // Update the physical to logical mapping with the new logical page number
    logicals[physicalPageNumber] = EncodeLogicalPageDescriptor(logicalPageNumber, pageProtectionLevel);

    return true;
}

auto ArchimedesMemc::FindReplacementPhysicalPage(
    const uint32_t physicalPageNumber,
    const uint32_t logicalPageNumber) -> std::optional<uint32_t> {
    for (auto i = 0u; i < logicals.size(); ++i) {
        if (physicalPageNumber == i) {
            continue;
        }
        if (ExtractLogicalPageDescriptorLogicalPageNumber(logicals[i]) == logicalPageNumber) {
            return { i };
        }
    }
    return {};
}

auto ArchimedesMemc::WriteAddressTranslator(
    const uint32_t physicalPageNumber,
    const uint32_t logicalPageNumber,
    const uint32_t pageProtectionLevel) -> void {

    // Update the logical to physical page mapping for the desired number of 4k pages
    const auto count = LogicalShiftLeft(1u, GetPageSize());
    const auto pageSizeBytes = GetPageSizeBytes();
    const auto physicalPageAddress = physicalPageNumber * pageSizeBytes;

    for (auto i = 0u; i < count; ++i) {
        physicals[logicalPageNumber * count + i] =
            EncodePhysicalPageDescriptor(
                physicalPageAddress + (i * 4096u),
                pageProtectionLevel);
    }
}

auto ArchimedesMemc::ClearAddressTranslator(const uint32_t logicalPageNumber) -> void {
    const auto count = LogicalShiftLeft(1u, GetPageSize());
    std::fill_n(physicals.begin() + logicalPageNumber * count, count, 0u);
}

auto ArchimedesMemc::TranslateReadLogicalAddress(uint32_t logicalAddress, uint32_t &physicalAddress) -> bool {
    const auto logicalPageNumber = ExtractBitField(logicalAddress, 12u, 13u);
    const auto logicalSubAddress = ExtractBitField(logicalAddress, 0, 12u);
    const auto descriptor = physicals[logicalPageNumber];
    if (!descriptor) {
        return false;
    }
    const auto physicalBaseAddress = ExtractPhysicalPageDescriptorAddress(descriptor);
    const auto pageProtectionLevel = ExtractPhysicalPageDescriptorPageProtectionLevel(descriptor);
    physicalAddress = physicalBaseAddress + logicalSubAddress;
    return readProtectionLevel >= pageProtectionLevel;
}

auto ArchimedesMemc::TranslateWriteLogicalAddress(uint32_t logicalAddress, uint32_t &physicalAddress) -> bool {
    const auto logicalPageNumber = ExtractBitField(logicalAddress, 12u, 13u);
    const auto logicalSubAddress = ExtractBitField(logicalAddress, 0, 12u);
    const auto descriptor = physicals[logicalPageNumber];
    if (!descriptor) {
        return false;
    }
    const auto physicalBaseAddress = ExtractPhysicalPageDescriptorAddress(descriptor);
    const auto pageProtectionLevel = ExtractPhysicalPageDescriptorPageProtectionLevel(descriptor);
    physicalAddress = physicalBaseAddress + logicalSubAddress;
    return writeProtectionLevel >= pageProtectionLevel;
}