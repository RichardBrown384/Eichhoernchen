#include "Archimedes/Archimedes.h"

#include "Archimedes/ArchimedesConfiguration.h"
#include "Archimedes/ArchimedesDiscImage.h"
#include "Archimedes/ArchimedesMediator.h"
#include "Archimedes/ArchimedesVidc.h"
#include "Archimedes/ArchimedesIoc.h"
#include "Archimedes/ArchimedesMemc.h"
#include "Archimedes/ArchimedesKeyboard.h"
#include "Archimedes/I2CController.h"
#include "Archimedes/RTC8583.h"
#include "Archimedes/WDC1772.h"

#include "Arm/Arm.h"

#include "Common/Platform/Platform.h"

#include <memory>

using namespace rbrown::platform;
using namespace rbrown::arm;

constexpr auto FOUR_MEGA_BYTES = 4u * 1024u * 1024u;

constexpr auto ExtractLatchAMotorField(const uint8_t v) { return ExtractBitField(~v, 5u, 1u); }
constexpr auto ExtractLatchASideField(const uint8_t v) { return ExtractBitField(~v, 4u, 1u); }
constexpr auto ExtractLatchADriveField(const uint8_t v) { return ExtractBitField(~v, 0u, 4u); }

constexpr auto ExtractLatchBResetField(const uint8_t v) { return ExtractBitField(~v, 3u, 1u); }
constexpr auto ExtractLatchBDensityField(const uint8_t v) { return ExtractBitField(~v, 1u, 1u); }

namespace rbrown::acorn::archimedes {

class Archimedes final: public Emulator, public ArchimedesMediator {
public:
    explicit Archimedes(Host&);
    ~Archimedes() override = default;
    
    Archimedes(const Archimedes&) = delete;
    Archimedes& operator=(const Archimedes&) = delete;

    auto ConstructObjects() -> bool;

    // Emulator interface
    auto KeyUp(ScanCode) -> void override;
    auto KeyDown(ScanCode) -> void override;
    auto MouseMotion(const MouseMotionData&) -> void override;
    auto MouseButtonUp(MouseButton) -> void override;
    auto MouseButtonDown(MouseButton) -> void override;
    auto StartUp() -> void override;
    auto ShutDown() -> void override;
    auto Reset() -> void override;
    [[nodiscard]] auto FrameTimeMillis() const -> int override { return 16; }
    auto Step() -> void override { m_arm->Execute(); }

    // Render Context Interface
    auto FrameCompleted() -> void override;
    auto WritePixel(uint32_t, uint32_t, uint32_t) -> void override;
    auto SetTextureSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void override;

    // IOC Interface
    auto ReadWordIoc(uint32_t) -> uint32_t override;
    auto WriteWordIoc(uint32_t, uint32_t) -> void override;

    // VIDC Interface
    auto WriteWordVidcRegister(uint32_t) -> void override;
    auto WriteWordVidcVideo(uint32_t) -> void override;
    auto WriteWordVidcCursor(uint32_t) -> void override;
    auto WriteWordVidcSound(uint32_t) -> void override;

    // Keyboard Interface
    auto ReadByteKeyboard(uint8_t& v) -> bool override;
    auto WriteByteKeyboard(uint8_t) -> void override;

    // I2C Interface
    auto ReadDataI2C() -> bool override;
    auto WriteClockDataI2C(uint32_t) -> void override;

    // Floppy Disc Interface
    auto ReadByteFloppyDiscController(uint32_t) -> uint8_t override;
    auto WriteByteFloppyDiscController(uint32_t, uint8_t) -> void override;

    // Latch A/B
    auto WriteByteLatchA(uint8_t) -> void override;
    auto WriteByteLatchB(uint8_t) -> void override;

    // System Events
    auto Update(uint32_t) -> void override;

    auto StartFlyBack() -> void override;
    auto EndFlyBack() -> void override;

    auto StartSoundInterrupt() -> void override;
    auto EndSoundInterrupt() -> void override;

    auto StartVideoDmaRequest() -> void override;
    auto StartCursorDmaRequest() -> void override;
    auto StartSoundDmaRequest() -> void override;

    auto EndVideoDmaRequest() -> void override;
    auto EndCursorDmaRequest() -> void override;
    auto EndSoundDmaRequest() -> void override;

    auto StartFloppyDiscDataRequest() -> void override;
    auto StartFloppyDiscInterruptRequest() -> void override;

    auto EndFloppyDiscDataRequest() -> void override;
    auto EndFloppyDiscInterruptRequest() -> void override;

    Host& m_host;

    ArchimedesKeyboard m_keyboard;

    std::unique_ptr<RTC8583> m_rtc;
    std::unique_ptr<I2CController> m_i2c;
    std::unique_ptr<DiscImage> m_discImage;
    std::unique_ptr<ArchimedesVidc> m_vidc;
    std::unique_ptr<ArchimedesIoc> m_ioc;
    std::unique_ptr<ArchimedesMemc> m_memc;
    std::unique_ptr<WDC1772> m_wdc1772;
    std::unique_ptr<Arm> m_arm;
};

Archimedes::Archimedes(Host& presentationContext) :
    m_host{presentationContext} {}

auto Archimedes::ConstructObjects() -> bool {
    auto lowRom = std::vector<uint8_t>{ 0u };
    auto highRom = m_host.LoadOperatingSystem();
    if (highRom.empty()) {
        return false;
    }
    m_discImage = CreateDiscImage(m_host.LoadMedia({}));
    if (!m_discImage) {
        return false;
    }
    auto cmos = m_host.LoadNonVolatileRam();
    m_rtc = std::make_unique<RTC8583>(std::move(cmos));
    if (!m_rtc) {
        return false;
    }
    m_i2c = std::make_unique<I2CController>(*m_rtc);
    m_vidc = std::make_unique<ArchimedesVidc>(*this);
    m_ioc = std::make_unique<ArchimedesIoc>(*this);
    m_memc = std::make_unique<ArchimedesMemc>(*this, FOUR_MEGA_BYTES, std::move(lowRom), std::move(highRom));
    m_wdc1772 = std::make_unique<WDC1772>(*this, *m_discImage);
    if (!m_i2c|| !m_vidc || !m_ioc || !m_memc || !m_wdc1772) {
        return false;
    }
    auto arm = std::make_unique<Arm>(*m_memc, *m_ioc);
    if (!arm) {
        return false;
    }
    m_arm.swap(arm);
    return true;
}

auto Archimedes::KeyDown(const ScanCode scanCode) -> void {
    if (const auto key = ArchimedesKeyboardConfiguration::MapScanCode(scanCode)) {
        m_keyboard.KeyDown(*key);
    }
}

auto Archimedes::KeyUp(const ScanCode scanCode) -> void {
    if (const auto key = ArchimedesKeyboardConfiguration::MapScanCode(scanCode)) {
        m_keyboard.KeyUp(*key);
    }
}

auto Archimedes::MouseMotion(const MouseMotionData& data) -> void {
    const auto&[x, y] = ArchimedesKeyboardConfiguration::MapMouseMotion(data.xrel, data.yrel);
    m_keyboard.MouseMotion(x, y);
}

auto Archimedes::MouseButtonDown(const MouseButton mouseButton) -> void {
    if (const auto button = ArchimedesKeyboardConfiguration::MapMouseButton(mouseButton)) {
        m_keyboard.MouseButtonDown(*button);
    }
}

auto Archimedes::MouseButtonUp(const MouseButton mouseButton) -> void {
    if (const auto button = ArchimedesKeyboardConfiguration::MapMouseButton(mouseButton)) {
        m_keyboard.MouseButtonUp(*button);
    }
}

auto Archimedes::StartUp() -> void {}

auto Archimedes::ShutDown() -> void {
    m_host.SaveNonVolatileRam(m_rtc->GetRam());
}

auto Archimedes::Reset() -> void {
    m_arm->Reset();
}

auto Archimedes::FrameCompleted() -> void {
    m_host.StopProducingFrame();
}

auto Archimedes::WritePixel(uint32_t x, uint32_t y, uint32_t c) -> void {
    m_host.SetPixel(x, y, c);
}

auto Archimedes::SetTextureSource(uint32_t x, uint32_t y, uint32_t w, uint32_t h) -> void {
    m_host.SetTextureSource(x, y, w, h);
}

auto Archimedes::ReadWordIoc(uint32_t address) -> uint32_t { return m_ioc->Read(address); }
auto Archimedes::WriteWordIoc(uint32_t address, uint32_t v) -> void { m_ioc->Write(address, v); }

auto Archimedes::WriteWordVidcRegister(uint32_t v) -> void { m_vidc->WriteWordRegister(v); }
auto Archimedes::WriteWordVidcVideo(uint32_t v) -> void { m_vidc->WriteWordVideo(v); }
auto Archimedes::WriteWordVidcCursor(uint32_t v) -> void { m_vidc->WriteWordCursor(v); }
auto Archimedes::WriteWordVidcSound(uint32_t) -> void {}

auto Archimedes::ReadByteKeyboard(uint8_t& v) -> bool { return m_keyboard.ReadByte(v); }
auto Archimedes::WriteByteKeyboard(uint8_t v) -> void { m_keyboard.WriteByte(v); }

auto Archimedes::ReadDataI2C() -> bool { return m_i2c->ReadData(); }
auto Archimedes::WriteClockDataI2C(uint32_t v) -> void { m_i2c->WriteClockData(v); }

auto Archimedes::ReadByteFloppyDiscController(uint32_t address) -> uint8_t {
    return m_wdc1772->ReadByteRegister(address);
}
auto Archimedes::WriteByteFloppyDiscController(uint32_t address, uint8_t v) -> void {
    m_wdc1772->WriteByteRegister(address, v);
}

auto Archimedes::WriteByteLatchA(uint8_t v) -> void {
    const auto drive = ExtractLatchADriveField(v);
    if (drive) {
        m_wdc1772->SetDrive(CountTrailingZeroes8(drive));
    }
    m_wdc1772->SetMotor(ExtractLatchAMotorField(v));
    m_wdc1772->SetSide(ExtractLatchASideField(v));
}
auto Archimedes::WriteByteLatchB(uint8_t v) -> void {
    const auto reset = ExtractLatchBResetField(v);
    if (reset) {
        m_wdc1772->Reset();
    }
    m_wdc1772->SetDensity(ExtractLatchBDensityField(v));
}

auto Archimedes::Update(uint32_t time) -> void {
    m_ioc->Update(time);
    m_vidc->Update(time);
    m_wdc1772->Update(time);
}

auto Archimedes::StartFlyBack() -> void { m_ioc->StartFlyBack(); }
auto Archimedes::EndFlyBack() -> void {
    m_ioc->EndFlyBack();
    m_memc->EndFlyBack();
}

auto Archimedes::StartSoundInterrupt() -> void { m_ioc->StartSoundInterrupt(); }
auto Archimedes::EndSoundInterrupt() -> void { m_ioc->EndSoundInterrupt(); }

auto Archimedes::StartVideoDmaRequest() -> void { m_memc->StartVideoDmaRequest(); }
auto Archimedes::StartCursorDmaRequest() -> void { m_memc->StartCursorDmaRequest(); }
auto Archimedes::StartSoundDmaRequest() -> void { m_memc->StartSoundDmaRequest(); }

auto Archimedes::EndVideoDmaRequest() -> void { }
auto Archimedes::EndCursorDmaRequest() -> void { }
auto Archimedes::EndSoundDmaRequest() -> void { }

auto Archimedes::StartFloppyDiscDataRequest() -> void { m_ioc->StartFloppyDiscDataRequest(); }
auto Archimedes::StartFloppyDiscInterruptRequest() -> void { m_ioc->StartFloppyDiscInterruptRequest(); }

auto Archimedes::EndFloppyDiscDataRequest() -> void { m_ioc->EndFloppyDiscDataRequest(); }
auto Archimedes::EndFloppyDiscInterruptRequest() -> void { m_ioc->EndFloppyDiscInterruptRequest(); }

auto CreateArchimedes(Host& host) -> std::unique_ptr<Emulator> {
    auto archimedes = std::make_unique<Archimedes>(host);
    if (!archimedes) {
        return {};
    }
    if (!archimedes->ConstructObjects()) {
        return {};
    }
    return archimedes;
}

auto GetArchimedesSystemProperties() -> SystemProperties {
    return SystemProperties{
        .displayProperties = {
            .windowTitle = "Archimedes",
            .logicalWidth = 640,
            .logicalHeight = 480,
            .textureWidth = 1152, // Mode 23
            .textureHeight = 896, // Mode 23
        },
        .inputProperties = {
            .needsRelativeMouseInput = true
        },
        .createEmulator = CreateArchimedes
    };
}
}