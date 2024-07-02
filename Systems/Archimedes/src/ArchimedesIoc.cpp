#include "Archimedes/ArchimedesIoc.h"
#include "Archimedes/ArchimedesMediator.h"

#include "Common/Util/BitUtil.h"

#include "spdlog/spdlog.h"

constexpr auto IOC_CHIP_FREQUENCY_MHZ = 2u;

constexpr auto MAX_IOC_TIMER_VALUE = 0xFFFFu;

constexpr auto CONTROL_FLY_BACK = 0x80U;

constexpr auto IRQ_A_FORCE_IRQ = 0x80u;
constexpr auto IRQ_A_TIMER_1_RELOADED = 0x40u;
constexpr auto IRQ_A_TIMER_0_RELOADED = 0x20u;
constexpr auto IRQ_A_POWER_ON_RESET = 0x10u;
constexpr auto IRQ_A_FLY_BACK = 0x08u;

constexpr auto IRQ_B_SERIAL_RECEIVE_FULL = 0x80u;
constexpr auto IRQ_B_SERIAL_TRANSMIT_EMPTY = 0x40u;
constexpr auto IRQ_B_SOUND_INTERRUPT = 0x02u;

constexpr auto FIQ_FORCE_FIQ = 0x80u;
constexpr auto FIQ_FLOPPY_DISC_INTERRUPT_REQUEST = 0x02u;
constexpr auto FIQ_FLOPPY_DISC_DATA_REQUEST = 0x01u;

[[maybe_unused]] constexpr auto CYCLE_SLOW = 0u;
[[maybe_unused]] constexpr auto CYCLE_MEDIUM = 1u;
[[maybe_unused]] constexpr auto CYCLE_FAST = 2u;
[[maybe_unused]] constexpr auto CYCLE_SYNC = 3u;

constexpr auto DEFAULT_REPLY = 0xFFFF'FFFFu;

constexpr auto ExtractChipSelect(uint32_t address) -> uint32_t { return ExtractBitField(address, 21u, 1u); }
constexpr auto ExtractCycleType(uint32_t address) -> uint32_t { return ExtractBitField(address, 19u, 2u); }
constexpr auto ExtractBank(uint32_t address) -> uint32_t { return ExtractBitField(address, 16u, 3u); }
constexpr auto ExtractInternalAddress(uint32_t address) -> uint32_t { return ExtractBitField(address, 2u, 5u) << 2u; }

constexpr std::array BANK_NAMES = {
    "internal", // 0
    "floppy disc", // 1
    "econet", // 2
    "serial port", // 3
    "internal expansion", // 4
    "hard disc", // 5
    "reserved", // 6
    "external expansion", // 7
};

namespace rbrown::acorn::archimedes {

ArchimedesIoc::ArchimedesIoc(ArchimedesMediator& m) :
    controlRegister{ 0xFFu },
    serialTransmitRegister{},
    serialReceiveRegister{},
    serialReceiveBuffer{},
    irqA{ IRQ_A_FORCE_IRQ | IRQ_A_POWER_ON_RESET },
    irqB{ IRQ_B_SERIAL_TRANSMIT_EMPTY },
    fiq{ FIQ_FORCE_FIQ },
    latchA{},
    latchB{},
    timer0{ MAX_IOC_TIMER_VALUE },
    timer1{ MAX_IOC_TIMER_VALUE },
    timer2{ MAX_IOC_TIMER_VALUE },
    timer3{ MAX_IOC_TIMER_VALUE },
    serialTransmitTimer{},
    serialReceiveTimer{},
    mediator{ m } {}

auto ArchimedesIoc::GetKeyboardByteTransmissionPeriod() const -> uint32_t {
    // The keyboard transmission timer rate is tied to timer 3
    // What I presume is the bit rate in MHz is given by 1 / ((latch + 1) * 16)
    // Transmitting a byte requires 11 bits (8 for the byte, plus one start, two stop)
    // To convert this into a timer we observe that at 2Mhz the period of a
    // countdown timer is latch/2 microseconds
    // Therefore we require 2 * (bits per byte) * (latch + 1) * 16 to give us the correct period
    return 2u * 11u * (timer3.ReadInputLatch() + 1u) * 16u;
}

auto ArchimedesIoc::Reset() -> void {}

auto ArchimedesIoc::PendingIrq() -> bool {
    return irqA.ReadRequest() | irqB.ReadRequest();
}

auto ArchimedesIoc::PendingFiq() -> bool {
    return fiq.ReadRequest();
}

auto ArchimedesIoc::Update(uint32_t time) -> void {
    const auto milliTicks = IOC_CHIP_FREQUENCY_MHZ * time;
    timer0.Update(milliTicks, [this]() -> void {
        irqA.SetBits(IRQ_A_TIMER_0_RELOADED);
    });
    timer1.Update(milliTicks, [this]() -> void {
        irqA.SetBits(IRQ_A_TIMER_1_RELOADED);
    });
    timer2.Update(milliTicks, []() -> void {});
    timer3.Update(milliTicks, []() -> void {});
    serialTransmitTimer.Update(milliTicks, [this]() -> void {
        mediator.WriteByteKeyboard(serialTransmitRegister);
        irqB.SetBits(IRQ_B_SERIAL_TRANSMIT_EMPTY);
    });
    serialReceiveTimer.Update(milliTicks, [this]() -> void {
        serialReceiveRegister = serialReceiveBuffer;
        irqB.SetBits(IRQ_B_SERIAL_RECEIVE_FULL);
    });
    if (serialTransmitTimer.Active() || serialReceiveTimer.Active()) {
        return;
    }
    if (!irqB.TestBits(IRQ_B_SERIAL_RECEIVE_FULL) && mediator.ReadByteKeyboard(serialReceiveBuffer)) {
        serialReceiveTimer.Go(GetKeyboardByteTransmissionPeriod());
    } else if (!irqB.TestBits(IRQ_B_SERIAL_TRANSMIT_EMPTY)) {
        serialTransmitTimer.Go(GetKeyboardByteTransmissionPeriod());
    }
}

auto ArchimedesIoc::StartFlyBack() -> void {
    controlRegister |= CONTROL_FLY_BACK;
    irqA.SetBits(IRQ_A_FLY_BACK);
}

auto ArchimedesIoc::EndFlyBack() -> void {
    controlRegister &= ~CONTROL_FLY_BACK;
}

auto ArchimedesIoc::StartSoundInterrupt() -> void {
    irqB.SetBits(IRQ_B_SOUND_INTERRUPT);
}

auto ArchimedesIoc::EndSoundInterrupt() -> void {
    irqB.ClearBits(IRQ_B_SOUND_INTERRUPT);
}

auto ArchimedesIoc::StartFloppyDiscDataRequest() -> void {
    fiq.SetBits(FIQ_FLOPPY_DISC_DATA_REQUEST);
}

auto ArchimedesIoc::StartFloppyDiscInterruptRequest() -> void {
    fiq.SetBits(FIQ_FLOPPY_DISC_INTERRUPT_REQUEST);
}

auto ArchimedesIoc::EndFloppyDiscDataRequest() -> void {
    fiq.ClearBits(FIQ_FLOPPY_DISC_DATA_REQUEST);
}

auto ArchimedesIoc::EndFloppyDiscInterruptRequest() -> void {
    fiq.ClearBits(FIQ_FLOPPY_DISC_INTERRUPT_REQUEST);
}

auto ArchimedesIoc::Read(uint32_t address) -> uint32_t {
    const auto chipSelect = ExtractChipSelect(address);
    const auto bank = ExtractBank(address);
    if (!chipSelect) {
        spdlog::debug("IOC read write with chip select clear {:08x}", address);
        return DEFAULT_REPLY;
    }
    switch (bank) {
        case 0u: return ReadInternalRegister(address);
        case 1u: return ReadBank1(address);
        default: {
            const auto cycleType = ExtractCycleType(address);
            spdlog::debug("IOC read from unsupported bank {} {} {:08x} cycle={}",
                         bank, BANK_NAMES[bank], address, cycleType);
            return DEFAULT_REPLY;
        }
    }
}

auto ArchimedesIoc::ReadInternalRegister(uint32_t address) -> uint32_t {
    const auto internalAddress = ExtractInternalAddress(address);
    switch (internalAddress) {
        case 0x00u: return ReadControlRegister();
        case 0x04u: return ReadSerialReceiveRegister();
        case 0x10u: return irqA.ReadStatus();
        case 0x14u: return irqA.ReadRequest();
        case 0x18u: return irqA.ReadMask();
        case 0x20u: return irqB.ReadStatus();
        case 0x24u: return irqB.ReadRequest();
        case 0x28u: return irqB.ReadMask();
        case 0x30u: return fiq.ReadStatus();
        case 0x34u: return fiq.ReadRequest();
        case 0x38u: return fiq.ReadMask();
        case 0x40u: return timer0.ReadOutputLatchLow();
        case 0x44u: return timer0.ReadOutputLatchHigh();
        case 0x50u: return timer1.ReadOutputLatchLow();
        case 0x54u: return timer1.ReadOutputLatchHigh();
        case 0x60u: return timer2.ReadOutputLatchLow();
        case 0x64u: return timer2.ReadOutputLatchHigh();
        case 0x70u: return timer3.ReadOutputLatchLow();
        case 0x74u: return timer3.ReadOutputLatchHigh();
        default: {
            spdlog::debug("IOC read from unsupported internal register address={:02x}", internalAddress);
            return DEFAULT_REPLY;
        }
    }
}

auto ArchimedesIoc::ReadControlRegister() const -> uint32_t {
    const auto data = mediator.ReadDataI2C();
    const auto result = (controlRegister & ~1u) + data;
    return result;
}

auto ArchimedesIoc::ReadSerialReceiveRegister() -> uint32_t {
    irqB.ClearBits(IRQ_B_SERIAL_RECEIVE_FULL);
    return serialReceiveRegister;
}

auto ArchimedesIoc::ReadBank1(uint32_t address) -> uint32_t {
    const auto cycleType = ExtractCycleType(address);
    const auto internalAddress = ExtractInternalAddress(address);
    if (cycleType != CYCLE_FAST) {
        spdlog::debug("IOC unsupported read from bank 1 {} {:08x} cycle={}",
                      BANK_NAMES[1], address, cycleType);
        return DEFAULT_REPLY;
    }
    switch (internalAddress) {
        case 0x00u: return mediator.ReadByteFloppyDiscController(0u);
        case 0x04u: return mediator.ReadByteFloppyDiscController(1u);
        case 0x08u: return mediator.ReadByteFloppyDiscController(2u);
        case 0x0Cu: return mediator.ReadByteFloppyDiscController(3u);
            [[unlikely]] default:
            spdlog::debug("IOC unsupported write to bank 1 {} {:08x} cycle={}",
                          BANK_NAMES[1], address, cycleType);
            return DEFAULT_REPLY;
    }
}

auto ArchimedesIoc::Write(uint32_t address, uint32_t dataBusValue) -> void {
    // A3000 Service manual says: During a WRITE (ie ARM to peripheral) D[16:31] is mapped to BD[0:15].
    const auto value = ExtractBitField(dataBusValue, 16u, 16u);
    const auto chipSelect = ExtractChipSelect(address);
    const auto bank = ExtractBank(address);
    if (!chipSelect) {
        spdlog::debug("IOC byte write with chip select clear {:08x}", address);
        return;
    }
    switch (bank) {
        case 0u: return WriteInternalRegister(address, value);
        case 1u: return WriteBank1(address, value);
        case 5u: return WriteBank5(address, value);
        default: {
            const auto cycleType = ExtractCycleType(address);
            spdlog::debug("IOC write to unsupported bank {} {} {:08x} cycle={}, {:02x}",
                          bank, BANK_NAMES[bank], address, cycleType, value);
        }
    }
}

auto ArchimedesIoc::WriteInternalRegister(uint32_t address, uint32_t value) -> void {
    value = ExtractBitField(value, 0u, 8u);
    const auto internalAddress = ExtractInternalAddress(address);
    switch (internalAddress) {
        case 0x00u: return WriteControlRegister(value);
        case 0x04u: return WriteSerialTransmitRegister(value);
        case 0x14u: return irqA.WriteClear(value & 0x7Cu);
        case 0x18u: return irqA.WriteMask(value);
        case 0x28u: return irqB.WriteMask(value);
        case 0x38u: return fiq.WriteMask(value);
        case 0x40u: return timer0.WriteInputLatchLow(value);
        case 0x44u: return timer0.WriteInputLatchHigh(value);
        case 0x48u: return timer0.WriteGoCommand();
        case 0x4Cu: return timer0.WriteLatchCommand();
        case 0x50u: return timer1.WriteInputLatchLow(value);
        case 0x54u: return timer1.WriteInputLatchHigh(value);
        case 0x58u: return timer1.WriteGoCommand();
        case 0x5Cu: return timer1.WriteLatchCommand();
        case 0x60u: return timer2.WriteInputLatchLow(value);
        case 0x64u: return timer2.WriteInputLatchHigh(value);
        case 0x68u: return timer2.WriteGoCommand();
        case 0x6Cu: return timer2.WriteLatchCommand();
        case 0x70u: return timer3.WriteInputLatchLow(value);
        case 0x74u: return timer3.WriteInputLatchHigh(value);
        case 0x78u: return timer3.WriteGoCommand();
        case 0x7Cu: return timer3.WriteLatchCommand();
        default: {
            spdlog::debug("IOC byte write to unsupported internal register address={:02x}, {:02x}", internalAddress,
                         value);
        }
    }
}

auto ArchimedesIoc::WriteControlRegister(uint32_t value) -> void {
    // The top two bits should always be written high
    controlRegister = ReplaceBitField(controlRegister, 0u, 6u, value);
    // Bits 3, 4 and 5 are mirrored (inverted) in the FIQ register
    fiq.ReplaceBits(3u, 3u, ~ExtractBitField(value, 3u, 3u));
    // Notify the I2C controller
    mediator.WriteClockDataI2C(value);
}

auto ArchimedesIoc::WriteSerialTransmitRegister(uint32_t value) -> void {
    serialTransmitRegister = value;
    irqB.ClearBits(IRQ_B_SERIAL_TRANSMIT_EMPTY);
    if (serialTransmitTimer.Active()) {
        serialTransmitTimer.Go(GetKeyboardByteTransmissionPeriod());
    }
}

auto ArchimedesIoc::WriteBank1(uint32_t address, uint32_t value) -> void {
    const auto cycleType = ExtractCycleType(address);
    const auto internalAddress = ExtractInternalAddress(address);
    value = ExtractBitField(value, 0u, 8u);
    if (cycleType != CYCLE_FAST) {
        spdlog::debug("IOC unsupported write to bank 1 {} {:08x} cycle={}, {:02x}",
                      BANK_NAMES[1], address, cycleType, value);
        return;
    }
    switch (internalAddress) {
        case 0x00u: return mediator.WriteByteFloppyDiscController(0u, value);
        case 0x04u: return mediator.WriteByteFloppyDiscController(1u, value);
        case 0x08u: return mediator.WriteByteFloppyDiscController(2u, value);
        case 0x0Cu: return mediator.WriteByteFloppyDiscController(3u, value);
            [[unlikely]] default:
            spdlog::debug("IOC unsupported write to bank 1 {} {:08x} cycle={}, {:02x}",
                          BANK_NAMES[1], address, cycleType, value);
    }
}

auto ArchimedesIoc::WriteBank5(uint32_t address, uint32_t value) -> void {
    const auto cycleType = ExtractCycleType(address);
    const auto internalAddress = ExtractInternalAddress(address);
    value = ExtractBitField(value, 0u, 8u);
    if (cycleType != CYCLE_FAST) {
        spdlog::debug("IOC unsupported write to bank 5 {} {:08x} cycle={}, {:02x}",
                      BANK_NAMES[5], address, cycleType, value);
       return;
    }
    switch (internalAddress) {
        case 0x18u: return WriteLatchB(value);
        case 0x40u: return WriteLatchA(value);
        default:
            spdlog::debug("IOC unsupported write to bank 5 {} {:08x} cycle={}, {:02x}",
                          BANK_NAMES[5], address, cycleType, value);
    }
}

auto ArchimedesIoc::WriteLatchA(uint32_t value) -> void {
    latchA = value;
    mediator.WriteByteLatchA(latchA);
}

auto ArchimedesIoc::WriteLatchB(uint32_t value) -> void {
    latchB = value;
    mediator.WriteByteLatchB(latchB);
}

}