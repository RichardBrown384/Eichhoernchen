#include "Frontend/Host/FrontendHost.h"

#include "Common/Platform/Platform.h"

#include "Frontend/FileSystem.h"
#include "Frontend/Toolkit.h"
#include "Frontend/Host/FrontendInput.h"

using ScanCode = rbrown::platform::ScanCode;
using GamepadButton = rbrown::platform::GamepadButton;
using Player = rbrown::platform::Player;

namespace rbrown::frontend {

class FrontendHost final : public platform::Host {
public:
    FrontendHost(
        const HostOptions& configuration,
        toolkit::Texture& texture) :
        m_options{configuration},
        m_texture{texture},
        m_producingFrame{} {
    }

    ~FrontendHost() override = default;

    // Disk I/O interactions
    auto LoadOperatingSystem() -> std::vector<uint8_t> override {
        const auto osDirectory = m_options.dataDirectory / "os";
        return filesystem::LoadMultipartFileFromDirectory(osDirectory);
    }

    auto LoadNonVolatileRam() -> std::vector<uint8_t> override {
        const auto nonVolatileRamPath = m_options.dataDirectory / "nvram.bin";
        return filesystem::LoadFile(nonVolatileRamPath);
    }

    auto LoadMedia(const std::vector<std::string>& extensions) -> std::vector<uint8_t> override {
        return filesystem::LoadZippedFileWithFallback(m_options.mediaPath, extensions);
    }

    auto SaveNonVolatileRam(const std::vector<uint8_t>& data) -> void override {
        const auto nonVolatileRamPath = m_options.dataDirectory / "nvram.bin";
        filesystem::SaveFile(nonVolatileRamPath, data);
    }

    // Keyboard interactions
    [[nodiscard]] auto GetKeyboardState(const ScanCode scanCode) const -> bool override {
        return input::GetKeyboardState(scanCode);
    }

    // Gamepad interactions
    [[nodiscard]] auto GetGamepadState(const Player player, const GamepadButton button) const -> bool override {
        return input::GetGamepadButton(player, button);
    }

    // Graphics interactions
    auto SetTextureSource(const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) -> void override {
        m_texture.SetSource(x, y, w, h);
    }

    auto SetPixel(const uint32_t x, const uint32_t y, const uint32_t c) -> void override {
        m_texture.SetPixel(x, y, c);
    }

    auto GetPixel(const uint32_t x, const uint32_t y) -> uint32_t override {
        return m_texture.GetPixel(x, y);
    }

    // Frame production
    auto StartProducingFrame() -> void override { m_producingFrame = true; }

    [[nodiscard]] auto IsProducingFrame() const -> bool override { return m_producingFrame; }

    auto StopProducingFrame() -> void override {
        m_producingFrame = false;
    }

private:
    const HostOptions& m_options;
    toolkit::Texture& m_texture;
    bool m_producingFrame;
};

auto CreateHost(const HostOptions& configuration,
                toolkit::Texture& texture) -> std::unique_ptr<platform::Host> {
    return std::make_unique<FrontendHost>(configuration, texture);
}
}
