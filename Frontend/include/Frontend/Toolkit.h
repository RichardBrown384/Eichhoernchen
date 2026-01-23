#pragma once

#include "Common/Platform/Platform.h"

#include <memory>

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Gamepad;

namespace rbrown::toolkit {

auto Init() -> bool;

auto Quit() -> void;

struct FloatRect {
    float x, y, w, h;
};

class Window {
    using Deleter = void(*)(SDL_Window*);
    using Handle = std::unique_ptr<SDL_Window, Deleter>;

public:
    explicit Window(Handle&& handle) noexcept;

    static auto Create(const char*, int, int) -> std::unique_ptr<Window>;

    [[nodiscard]] auto SetRelativeMouseMode(bool) const -> bool;
    [[nodiscard]] auto GetRelativeMouseMode() const -> bool;

    [[nodiscard]] auto Native() const -> SDL_Window*;

private:
    Handle m_handle;
};

class Renderer;

class Texture {
    using Deleter = void(*)(SDL_Texture*);
    using Handle = std::unique_ptr<SDL_Texture, Deleter>;

public:
    explicit Texture(Handle&& handle, int, int) noexcept;

    static auto Create(const Renderer&, int, int) -> std::unique_ptr<Texture>;

    auto SetSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void;
    [[nodiscard]] auto GetSource() const -> const FloatRect&;

    auto LockTexture() -> bool;

    auto UnlockTexture() -> void;

    auto SetPixel(uint32_t x, uint32_t y, uint32_t c) const -> void;

    [[nodiscard]] auto GetPixel(uint32_t x, uint32_t y) const -> uint32_t;

    [[nodiscard]] auto Native() const -> SDL_Texture*;

private:
    Handle m_handle;
    int m_width;
    int m_height;
    FloatRect m_source;
    unsigned char* m_pixels;
    int m_pitch;
};

class Renderer {
    using Deleter = void(*)(SDL_Renderer*);
    using Handle = std::unique_ptr<SDL_Renderer, Deleter>;

public:
    explicit Renderer(Handle&& handle) noexcept;

    static auto Create(const Window&) -> std::unique_ptr<Renderer>;

    [[nodiscard]] auto RenderTexture(const Texture&, const FloatRect&) const -> bool;
    [[nodiscard]] auto RenderTexture(const Texture&, const FloatRect&, const FloatRect&) const -> bool;

    [[nodiscard]] auto Present() const -> bool;

    [[nodiscard]] auto Native() const -> SDL_Renderer*;

private:
    Handle m_handle;
};

class Gamepads {
    using Deleter = void(*)(SDL_Gamepad*);
    using Handle = std::unique_ptr<SDL_Gamepad, Deleter>;

public:
    Gamepads() = default;

    Gamepads(const Gamepads&) = delete;

    Gamepads& operator=(const Gamepads&) = delete;

    auto Open(uint32_t) -> void;

    auto Close(uint32_t) -> void;

private:
    std::unordered_map<uint32_t, Handle> m_gamepads;
};

auto GetGamepadButton(platform::Player, platform::GamepadButton) -> bool;

auto GetKeyboardState(platform::ScanCode) -> bool;

enum class EventType: int {
    UNUSED,
    QUIT,
    WINDOW_RESIZED,
    WINDOW_FOCUS_GAINED,
    WINDOW_FOCUS_LOST,
    KEY_UP,
    KEY_DOWN,
    MOUSE_MOTION,
    MOUSE_BUTTON_UP,
    MOUSE_BUTTON_DOWN,
    GAMEPAD_ADDED,
    GAMEPAD_REMOVED,
};

struct WindowEvent {
    uint32_t which;
    int32_t data1;
    int32_t data2;
};

struct KeyboardEvent {
    uint8_t repeat;
    platform::ScanCode scanCode;
    bool down;
};

struct MouseMotionEvent {
    float x;
    float y;
    float xrel;
    float yrel;
};

struct MouseButtonEvent {
    platform::MouseButton button;
    bool down;
};

struct GamepadDeviceEvent {
    uint32_t which;
};

struct Event {
    EventType type;
    WindowEvent window;
    KeyboardEvent key;
    MouseMotionEvent motion;
    MouseButtonEvent button;
    GamepadDeviceEvent gdevice;
};

auto PollEvent(Event&) -> bool;
}
