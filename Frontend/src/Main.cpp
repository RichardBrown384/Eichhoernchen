#include "Frontend/LogicalViewport.h"
#include "Frontend/Toolkit.h"
#include "Frontend/CommandLine/ArgumentParser.h"
#include "Frontend/Host/FrontendHost.h"
#include "Frontend/Systems/Registry.h"
#include "Frontend/Systems/Systems.h"

#include "Archimedes/Archimedes.h"

#include <chrono>
#include <thread>

using namespace rbrown::toolkit;
using namespace rbrown::frontend;
using namespace rbrown::platform;

namespace {
auto Run(const Registry& registry, const ProgramArguments& arguments) -> int {

    const auto system = arguments.system;
    const auto mediaPath = arguments.mediaPath;
    const auto dataDirectory = arguments.dataDirectory;
    const auto scale = arguments.scale;

    // Check the system exists in the registry
    const auto systemProperties = registry.Get(system);
    if (!systemProperties) {
        return 1;
    }

    // Extract the display properties
    const auto& [
        windowTitle,
        logicalWidth,
        logicalHeight,
        textureWidth,
        textureHeight
    ] = systemProperties->displayProperties;

    // Create the window, exit on failure
    const auto windowWidth = scale * logicalWidth;
    const auto windowHeight = scale * logicalHeight;
    const auto window = Window::Create(
        windowTitle.c_str(),
        windowWidth,
        windowHeight
        );
    if (!window) {
        return 1;
    }

    // Create the renderer
    const auto renderer = Renderer::Create(*window);
    if (!renderer) {
        return 1;
    }

    // Create the texture
    const auto texture = Texture::Create(*renderer, textureWidth, textureHeight);
    if (!texture) {
        return 1;
    }

    // Construct the host
    const HostOptions hostOptions = {
        .mediaPath = mediaPath,
        .dataDirectory = dataDirectory,
    };
    auto host = CreateHost(hostOptions, *texture);
    if (!host) {
        return 1;
    }

    // Create the guest system/emulator
    const auto emulator = systemProperties->createEmulator(*host);
    if (!emulator) {
        return 1;
    }

    // Mouse input properties
    const auto needsRelativeMouseMode = systemProperties->inputProperties.needsRelativeMouseInput;

    // Initialise the gamepad manager
    Gamepads gamepads;

    // Set up the logical viewport
    LogicalViewport viewport(logicalWidth, logicalHeight);
    viewport.OnResize(windowWidth, windowHeight);

    emulator->StartUp();
    emulator->Reset();

    auto running = true;
    while (running) {
        Event event{};
        while (PollEvent(event)) {
            switch (event.type) {
                case EventType::QUIT:
                    running = false;
                    break;
                case EventType::WINDOW_RESIZED: {
                    const auto width = event.window.data1;
                    const auto height = event.window.data2;
                    viewport.OnResize(width, height);
                    break;
                }
                case EventType::WINDOW_FOCUS_LOST: {
                    if (window->GetRelativeMouseMode()) {
                        (void) window->SetRelativeMouseMode(false);
                    }
                    break;
                }
                case EventType::KEY_DOWN:
                    if (!event.key.repeat) {
                        emulator->KeyDown(event.key.scanCode);
                    }
                    break;
                case EventType::KEY_UP:
                    if (!event.key.repeat) {
                        emulator->KeyUp(event.key.scanCode);
                    }
                    break;
                case EventType::MOUSE_MOTION:
                    if (!needsRelativeMouseMode || window->GetRelativeMouseMode()) {
                        emulator->MouseMotion({
                                .x = event.motion.x,
                                .y = event.motion.y,
                                .xrel = event.motion.xrel,
                                .yrel = event.motion.yrel}
                            );
                    }
                    break;
                case EventType::MOUSE_BUTTON_DOWN:
                    if (!needsRelativeMouseMode || window->GetRelativeMouseMode()) {
                        emulator->MouseButtonDown(event.button.button);
                    }
                    break;
                case EventType::MOUSE_BUTTON_UP:
                    if (!needsRelativeMouseMode || window->GetRelativeMouseMode()) {
                        emulator->MouseButtonUp(event.button.button);
                    } else if (event.button.button == MouseButton::LEFT) {
                        (void) window->SetRelativeMouseMode(true);
                    }
                    break;
                case EventType::GAMEPAD_ADDED:
                    gamepads.Open(event.gdevice.which);
                    break;
                case EventType::GAMEPAD_REMOVED:
                    gamepads.Close(event.gdevice.which);
                    break;
                default:
                    break;
            }
        }

        const auto frameMillis = std::chrono::milliseconds(emulator->FrameTimeMillis());
        if (texture->LockTexture()) {
            const auto now = std::chrono::high_resolution_clock::now();
            const auto end = now + frameMillis;

            host->StartProducingFrame();
            emulator->OnFrame();

            while (host->IsProducingFrame()) [[likely]] {
                emulator->Step();
            }

            texture->UnlockTexture();
            (void) renderer->RenderTexture(*texture, viewport.GetRect());

            (void) renderer->Present();
            std::this_thread::sleep_until(end);
        }
    }

    emulator->ShutDown();

    return 0;
}
}

int main(const int argc, const char** argv) {

    using namespace rbrown;

    auto registry = Registry();
    registry.Register(ARCHIMEDES, acorn::archimedes::GetArchimedesSystemProperties());

    const auto& arguments = ParseArguments(argc, argv);

    if (!Init()) {
        return 1;
    }

    const auto exitCode = Run(registry, arguments);

    Quit();

    return exitCode;
}
