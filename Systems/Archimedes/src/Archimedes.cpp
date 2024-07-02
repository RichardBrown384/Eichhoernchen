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

#include "Toolkit/Toolkit.h"

#include <chrono>
#include <thread>
#include <memory>

using namespace rbrown::toolkit;
using namespace rbrown::arm;

constexpr auto FOUR_MEGA_BYTES = 4u * 1024u * 1024u;

constexpr auto ExtractLatchAMotorField(uint8_t v) { return ExtractBitField(~v, 5u, 1u); }
constexpr auto ExtractLatchASideField(uint8_t v) { return ExtractBitField(~v, 4u, 1u); }
constexpr auto ExtractLatchADriveField(uint8_t v) { return ExtractBitField(~v, 0u, 4u); }

constexpr auto ExtractLatchBResetField(uint8_t v) { return ExtractBitField(~v, 3u, 1u); }
constexpr auto ExtractLatchBDensityField(uint8_t v) { return ExtractBitField(~v, 1u, 1u); }

namespace rbrown::acorn::archimedes {

class Archimedes final: public Emulator, public ArchimedesMediator {
public:
    explicit Archimedes();
    ~Archimedes() final = default;
    
    Archimedes(const Archimedes&) = delete;
    Archimedes& operator=(const Archimedes&) = delete;

    auto ConstructObjects(const ArchimedesConfiguration&) -> bool;

    // Emulator interface
    auto KeyUp(const KeyboardEvent&) -> void final;
    auto KeyDown(const KeyboardEvent&) -> void final;
    auto MouseMotion(const MouseMotionEvent&) -> void final;
    auto MouseButtonUp(const MouseButtonEvent&) -> void final;
    auto MouseButtonDown(const MouseButtonEvent&) -> void final;
    auto StartUp() -> void final;
    auto ShutDown() -> void final;
    auto Reset() -> void final;
    auto RunForFrame() -> void final;

    // Render Context Interface
    auto FrameCompleted() -> void final;
    auto WritePixel(uint32_t, uint32_t, uint32_t) -> void final;
    auto SetTextureSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void final;

    // IOC Interface
    auto ReadWordIoc(uint32_t) -> uint32_t final;
    auto WriteWordIoc(uint32_t, uint32_t) -> void final;

    // VIDC Interface
    auto WriteWordVidcRegister(uint32_t) -> void final;
    auto WriteWordVidcVideo(uint32_t) -> void final;
    auto WriteWordVidcCursor(uint32_t) -> void final;
    auto WriteWordVidcSound(uint32_t) -> void final;

    // Keyboard Interface
    auto ReadByteKeyboard(uint8_t& v) -> bool final;
    auto WriteByteKeyboard(uint8_t) -> void final;

    // I2C Interface
    auto ReadDataI2C() -> bool final;
    auto WriteClockDataI2C(uint32_t) -> void final;

    // Floppy Disc Interface
    auto ReadByteFloppyDiscController(uint32_t) -> uint8_t final;
    auto WriteByteFloppyDiscController(uint32_t, uint8_t) -> void final;

    // Latch A/B
    auto WriteByteLatchA(uint8_t) -> void final;
    auto WriteByteLatchB(uint8_t) -> void final;

    // System Events
    auto Update(uint32_t) -> void final;

    auto StartFlyBack() -> void final;
    auto EndFlyBack() -> void final;

    auto StartSoundInterrupt() -> void final;
    auto EndSoundInterrupt() -> void final;

    auto StartVideoDmaRequest() -> void final;
    auto StartCursorDmaRequest() -> void final;
    auto StartSoundDmaRequest() -> void final;

    auto EndVideoDmaRequest() -> void final;
    auto EndCursorDmaRequest() -> void final;
    auto EndSoundDmaRequest() -> void final;

    auto StartFloppyDiscDataRequest() -> void final;
    auto StartFloppyDiscInterruptRequest() -> void final;

    auto EndFloppyDiscDataRequest() -> void final;
    auto EndFloppyDiscInterruptRequest() -> void final;

    RTC8583 m_rtc;
    I2CController m_i2c;
    ArchimedesKeyboard m_keyboard;

    std::unique_ptr<PresentationContext> m_presentationContext;
    std::unique_ptr<DiscImage> m_discImage;
    std::unique_ptr<ArchimedesVidc> m_vidc;
    std::unique_ptr<ArchimedesIoc> m_ioc;
    std::unique_ptr<ArchimedesMemc> m_memc;
    std::unique_ptr<WDC1772> m_wdc1772;
    std::unique_ptr<Arm> m_arm;
};

Archimedes::Archimedes() :
    m_rtc{},
    m_i2c{ m_rtc },
    m_keyboard{},
    m_presentationContext{},
    m_discImage{},
    m_vidc{},
    m_ioc{},
    m_memc{},
    m_wdc1772{},
    m_arm{} {}

auto Archimedes::ConstructObjects(const ArchimedesConfiguration& config) -> bool {
    m_presentationContext = rbrown::toolkit::CreatePresentationContext("Archimedes", config.m_config.m_scale, 800, 600);
    if (!m_presentationContext) {
        return false;
    }
    auto lowRom = std::vector<uint8_t>{ 0u };
    auto highRom = LoadMultipartFile(config.m_config.m_biosFiles);
    if (highRom.empty()) {
        return false;
    }
    m_discImage = CreateDiscImage(LoadFile(config.m_config.m_filename));
    if (!m_discImage) {
        return false;
    }
    m_vidc = std::make_unique<ArchimedesVidc>(*this);
    m_ioc = std::make_unique<ArchimedesIoc>(*this);
    m_memc = std::make_unique<ArchimedesMemc>(*this, FOUR_MEGA_BYTES, std::move(lowRom), std::move(highRom));
    m_wdc1772 = std::make_unique<WDC1772>(*this, *m_discImage);
    if (!m_vidc || !m_ioc || !m_memc || !m_wdc1772) {
        return false;
    }
    auto arm = std::make_unique<rbrown::arm::Arm>(*m_memc, *m_ioc);
    if (!arm) {
        return false;
    }
    m_arm.swap(arm);
    return true;
}

auto Archimedes::KeyDown(const KeyboardEvent& e) -> void {
    const auto key = ArchimedesKeyboardConfiguration::MapScanCode(e.scanCode);
    if (!e.repeat && key) { m_keyboard.KeyDown(*key); }
}

auto Archimedes::KeyUp(const KeyboardEvent& e) -> void {
    const auto key = ArchimedesKeyboardConfiguration::MapScanCode(e.scanCode);
    if (!e.repeat && key) { m_keyboard.KeyUp(*key); }
}

auto Archimedes::MouseMotion(const MouseMotionEvent& e) -> void {
    const auto&[x, y] = ArchimedesKeyboardConfiguration::MapMouseMotion(e.xrel, e.yrel);
    m_keyboard.MouseMotion(x, y);
}

auto Archimedes::MouseButtonDown(const MouseButtonEvent& e) -> void {
    const auto button = ArchimedesKeyboardConfiguration::MapMouseButton(e.button);
    if (button) { m_keyboard.MouseButtonDown(*button); }
}

auto Archimedes::MouseButtonUp(const MouseButtonEvent& e) -> void {
    const auto button = ArchimedesKeyboardConfiguration::MapMouseButton(e.button);
    if (button) { m_keyboard.MouseButtonUp(*button); }
}

auto Archimedes::StartUp() -> void {
    m_presentationContext->SetWindowSize(640, 512);
    SetRelativeMouseMode(true);
}

auto Archimedes::ShutDown() -> void {}

auto Archimedes::Reset() -> void {
    m_arm->Reset();
}

auto Archimedes::RunForFrame() -> void {
    constexpr static auto frameMillis = std::chrono::milliseconds(16);
    if (m_presentationContext->LockTexture()) {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto end = now + frameMillis;
        m_presentationContext->StartFrame();
        while (!m_presentationContext->IsFrameComplete()) {
            /*const auto pc = m_arm->GetPC();
            const auto instruction = m_arm->GetDecodedInstruction();
            const auto dasm = rbrown::arm::Disassemble(instruction);
            spdlog::debug("{:08x} : {}", pc - 8u, dasm);*/
            m_arm->Execute();
        }
        m_presentationContext->UnlockTexture();
        m_presentationContext->Present();
        std::this_thread::sleep_until(end);
    }
}

auto Archimedes::FrameCompleted() -> void {
    m_presentationContext->CompleteFrame();
}

auto Archimedes::WritePixel(uint32_t x, uint32_t y, uint32_t c) -> void {
    m_presentationContext->SetPixel(x, y, c);
}

auto Archimedes::SetTextureSource(uint32_t x, uint32_t y, uint32_t w, uint32_t h) -> void {
    m_presentationContext->SetTextureSource(x, y, w, h);
}

auto Archimedes::ReadWordIoc(uint32_t address) -> uint32_t { return m_ioc->Read(address); }
auto Archimedes::WriteWordIoc(uint32_t address, uint32_t v) -> void { m_ioc->Write(address, v); }

auto Archimedes::WriteWordVidcRegister(uint32_t v) -> void { m_vidc->WriteWordRegister(v); }
auto Archimedes::WriteWordVidcVideo(uint32_t v) -> void { m_vidc->WriteWordVideo(v); }
auto Archimedes::WriteWordVidcCursor(uint32_t v) -> void { m_vidc->WriteWordCursor(v); }
auto Archimedes::WriteWordVidcSound(uint32_t) -> void {}

auto Archimedes::ReadByteKeyboard(uint8_t& v) -> bool { return m_keyboard.ReadByte(v); }
auto Archimedes::WriteByteKeyboard(uint8_t v) -> void { m_keyboard.WriteByte(v); }

auto Archimedes::ReadDataI2C() -> bool { return m_i2c.ReadData(); }
auto Archimedes::WriteClockDataI2C(uint32_t v) -> void { m_i2c.WriteClockData(v); }

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

auto CreateArchimedes(const ArchimedesConfiguration& config) -> std::unique_ptr<Emulator> {
    auto archimedes = std::make_unique<Archimedes>();
    if (!archimedes) {
        return {};
    }
    if (!archimedes->ConstructObjects(config)) {
        return {};
    }
    return archimedes;
}
}