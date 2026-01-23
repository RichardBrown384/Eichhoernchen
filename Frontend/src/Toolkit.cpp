#include "Frontend/Toolkit.h"

#include <SDL3/SDL.h>

#include "Frontend/InputBindingUtil.h"

#include <array>
#include <utility>

using namespace rbrown::platform;
using namespace rbrown::frontend::binding;

namespace rbrown::toolkit {

constexpr std::array KEYBOARD_BINDINGS = {
    KV(ScanCode::CODE_UNKNOWN, SDL_SCANCODE_UNKNOWN),

    KV(ScanCode::CODE_A, SDL_SCANCODE_A),
    KV(ScanCode::CODE_B, SDL_SCANCODE_B),
    KV(ScanCode::CODE_C, SDL_SCANCODE_C),
    KV(ScanCode::CODE_D, SDL_SCANCODE_D),
    KV(ScanCode::CODE_E, SDL_SCANCODE_E),
    KV(ScanCode::CODE_F, SDL_SCANCODE_F),
    KV(ScanCode::CODE_G, SDL_SCANCODE_G),
    KV(ScanCode::CODE_H, SDL_SCANCODE_H),
    KV(ScanCode::CODE_I, SDL_SCANCODE_I),
    KV(ScanCode::CODE_J, SDL_SCANCODE_J),
    KV(ScanCode::CODE_K, SDL_SCANCODE_K),
    KV(ScanCode::CODE_L, SDL_SCANCODE_L),
    KV(ScanCode::CODE_M, SDL_SCANCODE_M),
    KV(ScanCode::CODE_N, SDL_SCANCODE_N),
    KV(ScanCode::CODE_O, SDL_SCANCODE_O),
    KV(ScanCode::CODE_P, SDL_SCANCODE_P),
    KV(ScanCode::CODE_Q, SDL_SCANCODE_Q),
    KV(ScanCode::CODE_R, SDL_SCANCODE_R),
    KV(ScanCode::CODE_S, SDL_SCANCODE_S),
    KV(ScanCode::CODE_T, SDL_SCANCODE_T),
    KV(ScanCode::CODE_U, SDL_SCANCODE_U),
    KV(ScanCode::CODE_V, SDL_SCANCODE_V),
    KV(ScanCode::CODE_W, SDL_SCANCODE_W),
    KV(ScanCode::CODE_X, SDL_SCANCODE_X),
    KV(ScanCode::CODE_Y, SDL_SCANCODE_Y),
    KV(ScanCode::CODE_Z, SDL_SCANCODE_Z),

    KV(ScanCode::CODE_1, SDL_SCANCODE_1),
    KV(ScanCode::CODE_2, SDL_SCANCODE_2),
    KV(ScanCode::CODE_3, SDL_SCANCODE_3),
    KV(ScanCode::CODE_4, SDL_SCANCODE_4),
    KV(ScanCode::CODE_5, SDL_SCANCODE_5),
    KV(ScanCode::CODE_6, SDL_SCANCODE_6),
    KV(ScanCode::CODE_7, SDL_SCANCODE_7),
    KV(ScanCode::CODE_8, SDL_SCANCODE_8),
    KV(ScanCode::CODE_9, SDL_SCANCODE_9),
    KV(ScanCode::CODE_0, SDL_SCANCODE_0),

    KV(ScanCode::CODE_RETURN, SDL_SCANCODE_RETURN),
    KV(ScanCode::CODE_ESCAPE, SDL_SCANCODE_ESCAPE),
    KV(ScanCode::CODE_BACKSPACE, SDL_SCANCODE_BACKSPACE),
    KV(ScanCode::CODE_TAB, SDL_SCANCODE_TAB),
    KV(ScanCode::CODE_SPACE, SDL_SCANCODE_SPACE),

    KV(ScanCode::CODE_MINUS, SDL_SCANCODE_MINUS),
    KV(ScanCode::CODE_EQUALS, SDL_SCANCODE_EQUALS),
    KV(ScanCode::CODE_LEFTBRACKET, SDL_SCANCODE_LEFTBRACKET),
    KV(ScanCode::CODE_RIGHTBRACKET, SDL_SCANCODE_RIGHTBRACKET),
    KV(ScanCode::CODE_BACKSLASH, SDL_SCANCODE_BACKSLASH),
    KV(ScanCode::CODE_SEMICOLON, SDL_SCANCODE_SEMICOLON),
    KV(ScanCode::CODE_APOSTROPHE, SDL_SCANCODE_APOSTROPHE),
    KV(ScanCode::CODE_GRAVE, SDL_SCANCODE_GRAVE),
    KV(ScanCode::CODE_COMMA, SDL_SCANCODE_COMMA),
    KV(ScanCode::CODE_PERIOD, SDL_SCANCODE_PERIOD),
    KV(ScanCode::CODE_SLASH, SDL_SCANCODE_SLASH),

    KV(ScanCode::CODE_CAPSLOCK, SDL_SCANCODE_CAPSLOCK),

    KV(ScanCode::CODE_F1, SDL_SCANCODE_F1),
    KV(ScanCode::CODE_F2, SDL_SCANCODE_F2),
    KV(ScanCode::CODE_F3, SDL_SCANCODE_F3),
    KV(ScanCode::CODE_F4, SDL_SCANCODE_F4),
    KV(ScanCode::CODE_F5, SDL_SCANCODE_F5),
    KV(ScanCode::CODE_F6, SDL_SCANCODE_F6),
    KV(ScanCode::CODE_F7, SDL_SCANCODE_F7),
    KV(ScanCode::CODE_F8, SDL_SCANCODE_F8),
    KV(ScanCode::CODE_F9, SDL_SCANCODE_F9),
    KV(ScanCode::CODE_F10, SDL_SCANCODE_F10),
    KV(ScanCode::CODE_F11, SDL_SCANCODE_F11),
    KV(ScanCode::CODE_F12, SDL_SCANCODE_F12),

    KV(ScanCode::CODE_PRINTSCREEN, SDL_SCANCODE_PRINTSCREEN),
    KV(ScanCode::CODE_SCROLLLOCK, SDL_SCANCODE_SCROLLLOCK),
    KV(ScanCode::CODE_PAUSE, SDL_SCANCODE_PAUSE),
    KV(ScanCode::CODE_INSERT, SDL_SCANCODE_INSERT),

    KV(ScanCode::CODE_HOME, SDL_SCANCODE_HOME),
    KV(ScanCode::CODE_PAGEUP, SDL_SCANCODE_PAGEUP),
    KV(ScanCode::CODE_DELETE, SDL_SCANCODE_DELETE),
    KV(ScanCode::CODE_END, SDL_SCANCODE_END),
    KV(ScanCode::CODE_PAGEDOWN, SDL_SCANCODE_PAGEDOWN),
    KV(ScanCode::CODE_RIGHT, SDL_SCANCODE_RIGHT),
    KV(ScanCode::CODE_LEFT, SDL_SCANCODE_LEFT),
    KV(ScanCode::CODE_DOWN, SDL_SCANCODE_DOWN),
    KV(ScanCode::CODE_UP, SDL_SCANCODE_UP),

    KV(ScanCode::CODE_NUMLOCKCLEAR, SDL_SCANCODE_NUMLOCKCLEAR),

    KV(ScanCode::CODE_KP_DIVIDE, SDL_SCANCODE_KP_DIVIDE),
    KV(ScanCode::CODE_KP_MULTIPLY, SDL_SCANCODE_KP_MULTIPLY),
    KV(ScanCode::CODE_KP_MINUS, SDL_SCANCODE_KP_MINUS),
    KV(ScanCode::CODE_KP_PLUS, SDL_SCANCODE_KP_PLUS),
    KV(ScanCode::CODE_KP_ENTER, SDL_SCANCODE_KP_ENTER),
    KV(ScanCode::CODE_KP_1, SDL_SCANCODE_KP_1),
    KV(ScanCode::CODE_KP_2, SDL_SCANCODE_KP_2),
    KV(ScanCode::CODE_KP_3, SDL_SCANCODE_KP_3),
    KV(ScanCode::CODE_KP_4, SDL_SCANCODE_KP_4),
    KV(ScanCode::CODE_KP_5, SDL_SCANCODE_KP_5),
    KV(ScanCode::CODE_KP_6, SDL_SCANCODE_KP_6),
    KV(ScanCode::CODE_KP_7, SDL_SCANCODE_KP_7),
    KV(ScanCode::CODE_KP_8, SDL_SCANCODE_KP_8),
    KV(ScanCode::CODE_KP_9, SDL_SCANCODE_KP_9),
    KV(ScanCode::CODE_KP_0, SDL_SCANCODE_KP_0),
    KV(ScanCode::CODE_KP_PERIOD, SDL_SCANCODE_KP_PERIOD),
    KV(ScanCode::CODE_KP_COMMA, SDL_SCANCODE_KP_COMMA),

    KV(ScanCode::CODE_LCTRL, SDL_SCANCODE_LCTRL),
    KV(ScanCode::CODE_LSHIFT, SDL_SCANCODE_LSHIFT),
    KV(ScanCode::CODE_LALT, SDL_SCANCODE_LALT),
    KV(ScanCode::CODE_LGUI, SDL_SCANCODE_LGUI),
    KV(ScanCode::CODE_RCTRL, SDL_SCANCODE_RCTRL),
    KV(ScanCode::CODE_RSHIFT, SDL_SCANCODE_RSHIFT),
    KV(ScanCode::CODE_RALT, SDL_SCANCODE_RALT),
    KV(ScanCode::CODE_RGUI, SDL_SCANCODE_RGUI),
};

static_assert(IsBijection(KEYBOARD_BINDINGS),
              "Keyboard bindings must be a bijection");
static_assert(KEYBOARD_BINDINGS.size() == static_cast<size_t>(ScanCode::CODE_MAX),
              "Keyboard bindings must map all logical inputs to backend equivalents");

constexpr std::array GAMEPAD_BINDINGS = {
    KV(GamepadButton::GAMEPAD_UNKNOWN, SDL_GAMEPAD_BUTTON_INVALID),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_UP, SDL_GAMEPAD_BUTTON_DPAD_UP),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_DOWN),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_LEFT, SDL_GAMEPAD_BUTTON_DPAD_LEFT),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_RIGHT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT),
    KV(GamepadButton::GAMEPAD_BUTTON_NORTH, SDL_GAMEPAD_BUTTON_NORTH),
    KV(GamepadButton::GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_SOUTH),
    KV(GamepadButton::GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_EAST),
    KV(GamepadButton::GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_WEST),
    KV(GamepadButton::GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_START),
    KV(GamepadButton::GAMEPAD_BUTTON_BACK, SDL_GAMEPAD_BUTTON_BACK)
};

static_assert(IsBijection(GAMEPAD_BINDINGS),
              "Gamepad bindings must be a bijection");
static_assert(GAMEPAD_BINDINGS.size() == static_cast<size_t>(GamepadButton::GAMEPAD_MAX),
              "Gamepad bindings must map all logical inputs to backend equivalents");

constexpr std::array MOUSE_BINDINGS = {
    KV(MouseButton::LEFT, SDL_BUTTON_LEFT),
    KV(MouseButton::MIDDLE, SDL_BUTTON_MIDDLE),
    KV(MouseButton::RIGHT, SDL_BUTTON_RIGHT),
    KV(MouseButton::SIDE_1, SDL_BUTTON_X1),
    KV(MouseButton::SIDE_2, SDL_BUTTON_X2)
};

// SDL has no 'unknown' code for a mouse event so we can't ensure that everything is mapped
static_assert(IsBijection(MOUSE_BINDINGS),
              "Mouse bindings must be a bijection");

// From us to SDL
constexpr auto GAMEPAD_FORWARD_LOOKUP = MakeForwardLookup(GAMEPAD_BINDINGS, SDL_GAMEPAD_BUTTON_INVALID);
constexpr auto KEYBOARD_FORWARD_LOOKUP = MakeForwardLookup(KEYBOARD_BINDINGS, SDL_SCANCODE_UNKNOWN);

// From SDL to us (when we receive an event we need to tell the guest which of our scancodes was used)
constexpr auto KEYBOARD_REVERSE_LOOKUP = MakeReverseLookup<SDL_SCANCODE_COUNT>(
    KEYBOARD_BINDINGS, ScanCode::CODE_UNKNOWN);
constexpr auto MOUSE_REVERSE_LOOKUP = MakeReverseLookup<8u/* SDL only defines 5 mouse buttons*/>(
    MOUSE_BINDINGS, MouseButton::UNKNOWN);

auto Init() -> bool { return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD); }
auto Quit() -> void { SDL_Quit(); }

auto Window::Create(const char* title, const int w, const int h) -> std::unique_ptr<Window> {
    auto handle = Handle{
        SDL_CreateWindow(title, w, h,SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL),
        SDL_DestroyWindow
    };
    if (!handle) {
        return nullptr;
    }
    return std::make_unique<Window>(std::move(handle));
}

Window::Window(Handle&& handle) noexcept :
    m_handle(std::move(handle)) {
}

auto Window::SetRelativeMouseMode(const bool enabled) const -> bool {
    return SDL_SetWindowRelativeMouseMode(Native(), enabled);
}

auto Window::GetRelativeMouseMode() const -> bool {
    return SDL_GetWindowRelativeMouseMode(Native());
}

auto Window::Native() const -> SDL_Window* {
    return m_handle.get();
}

auto Texture::Create(const Renderer& renderer, const int w, const int h) -> std::unique_ptr<Texture> {
    auto handle = Handle{
        SDL_CreateTexture(
            renderer.Native(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            w,
            h),
        SDL_DestroyTexture
    };
    if (!handle) {
        return nullptr;
    }
    SDL_SetTextureBlendMode(handle.get(), SDL_BLENDMODE_NONE);
    SDL_SetTextureScaleMode(handle.get(), SDL_SCALEMODE_NEAREST);
    return std::make_unique<Texture>(std::move(handle), w, h);
}

Texture::Texture(Handle&& handle, const int w, const int h) noexcept :
    m_handle(std::move(handle)),
    m_width{w},
    m_height{h},
    m_source{
        .x = 0,
        .y = 0,
        .w = static_cast<float>(w),
        .h = static_cast<float>(h)
    },
    m_pixels{},
    m_pitch{} {
}

auto Texture::SetSource(const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) -> void {
    m_source.x = std::max(static_cast<float>(x), 0.0f);
    m_source.y = std::max(static_cast<float>(y), 0.0f);
    m_source.w = std::min(static_cast<float>(w), static_cast<float>(m_width));
    m_source.h = std::min(static_cast<float>(h), static_cast<float>(m_height));
}

auto Texture::GetSource() const -> const FloatRect& { return m_source; }

auto Texture::LockTexture() -> bool {
    if (!SDL_LockTexture(Native(), nullptr, reinterpret_cast<void**>(&m_pixels), &m_pitch)) {
        m_pixels = {};
        m_pitch = {};
        return false;
    }
    return true;
}

auto Texture::UnlockTexture() -> void {
    SDL_UnlockTexture(Native());
    m_pixels = {};
    m_pitch = {};
}

auto Texture::SetPixel(const uint32_t x, const uint32_t y, const uint32_t c) const -> void {
    if (m_pixels && x < static_cast<uint32_t>(m_width) && y < static_cast<uint32_t>(m_height)) [[likely]] {
        reinterpret_cast<uint32_t*>(m_pixels + y * m_pitch)[x] = c;
    }
}

auto Texture::GetPixel(const uint32_t x, const uint32_t y) const -> uint32_t {
    if (m_pixels && x < static_cast<uint32_t>(m_width) && y < static_cast<uint32_t>(m_height)) [[likely]] {
        return reinterpret_cast<uint32_t*>(m_pixels + y * m_pitch)[x];
    }
    return 0u;
}

auto Texture::Native() const -> SDL_Texture* {
    return m_handle.get();
}

auto Renderer::Create(const Window& window) -> std::unique_ptr<Renderer> {
    auto handle = Handle{
        SDL_CreateRenderer(window.Native(), nullptr),
        SDL_DestroyRenderer
    };
    if (!handle) {
        return nullptr;
    }
    return std::make_unique<Renderer>(std::move(handle));
}

Renderer::Renderer(Handle&& handle) noexcept :
    m_handle(std::move(handle)) {
}

auto Renderer::RenderTexture(const Texture& t, const FloatRect& d) const -> bool {
    return RenderTexture(t, t.GetSource(), d);
}

auto Renderer::RenderTexture(const Texture& t, const FloatRect& s, const FloatRect& d) const -> bool {
    const SDL_FRect source = {.x = s.x, .y = s.y, .w = s.w, .h = s.h};
    const SDL_FRect dest = {.x = d.x, .y = d.y, .w = d.w, .h = d.h};
    return SDL_RenderTexture(Native(), t.Native(), &source, &dest);
}

auto Renderer::Present() const -> bool {
    return SDL_RenderPresent(Native());
}

auto Renderer::Native() const -> SDL_Renderer* {
    return m_handle.get();
}

auto Gamepads::Open(const uint32_t joystickId) -> void {
    if (m_gamepads.contains(joystickId)) {
        return;
    }
    const auto gamepad = SDL_OpenGamepad(joystickId);
    if (!gamepad) {
        return;
    }
    m_gamepads.emplace(joystickId, Handle{gamepad, SDL_CloseGamepad});
}

auto Gamepads::Close(const uint32_t joystickId) -> void {
    m_gamepads.erase(joystickId);
}

auto MapSDLScanCodeToToolkitScanCode(const int sdlScanCode) -> ScanCode {
    if (sdlScanCode >= 0 && static_cast<uint32_t>(sdlScanCode) < KEYBOARD_REVERSE_LOOKUP.size()) {
        return KEYBOARD_REVERSE_LOOKUP[sdlScanCode];
    }
    return ScanCode::CODE_UNKNOWN;
}

auto MapToolkitScanCodeToSDLScanCode(const ScanCode scanCode) -> int {
    if (const auto index = static_cast<uint32_t>(U(scanCode)); index < KEYBOARD_FORWARD_LOOKUP.size()) {
        return KEYBOARD_FORWARD_LOOKUP[index];
    }
    return SDL_SCANCODE_UNKNOWN;
}

auto GetKeyboardState(const ScanCode c) -> bool {
    int numKeys{};
    const auto sdlScanCode = MapToolkitScanCodeToSDLScanCode(c);
    if (const auto state = SDL_GetKeyboardState(&numKeys); state && sdlScanCode < numKeys) {
        return state[sdlScanCode];
    }
    return false;
}

auto GetGamepadButton(const Player player, const GamepadButton button) -> bool {
    if (const auto gamepad = SDL_GetGamepadFromPlayerIndex(U(player))) {
        return SDL_GetGamepadButton(gamepad, GAMEPAD_FORWARD_LOOKUP[U(button)]);
    }
    return false;
}

auto MapSDLMouseButtonCodeToToolkitMouseButton(const uint8_t sdlButtonCode) -> MouseButton {
    if (sdlButtonCode < MOUSE_REVERSE_LOOKUP.size()) {
        return MOUSE_REVERSE_LOOKUP[sdlButtonCode];
    }
    return MouseButton::UNKNOWN;
}

auto MapQuitEvent(const EventType type, const SDL_Event&, Event& e) -> bool {
    e.type = type;
    return true;
}

auto MapWindowEvent(const EventType type, const SDL_Event& sdlEvent, Event& e) -> bool {
    e.type = type;
    e.window.which = sdlEvent.window.windowID;
    e.window.data1 = sdlEvent.window.data1;
    e.window.data2 = sdlEvent.window.data2;
    return true;
}

auto MapKeyEvent(const EventType type, const SDL_Event& sdlEvent, Event& e) -> bool {
    if (const auto scanCode = MapSDLScanCodeToToolkitScanCode(sdlEvent.key.scancode);
        scanCode != ScanCode::CODE_UNKNOWN) {
        e.type = type;
        e.key.scanCode = scanCode;
        e.key.repeat = sdlEvent.key.repeat;
        e.key.down = sdlEvent.key.down;
    }
    return true;
}

auto MapMouseMotionEvent(const SDL_Event& sdlEvent, Event& e) -> bool {
    e.type = EventType::MOUSE_MOTION;
    e.motion.x = sdlEvent.motion.x;
    e.motion.y = sdlEvent.motion.y;
    e.motion.xrel = sdlEvent.motion.xrel;
    e.motion.yrel = sdlEvent.motion.yrel;
    return true;
}

auto MapMouseButtonEvent(const EventType type, const SDL_Event& sdlEvent, Event& e) -> bool {
    if (const auto mouseButton = MapSDLMouseButtonCodeToToolkitMouseButton(sdlEvent.button.button);
        mouseButton != MouseButton::UNKNOWN) {
        e.type = type;
        e.button.button = mouseButton;
        e.button.down = sdlEvent.button.down;
    }
    return true;
}

auto MapGamepadDeviceEvent(const EventType type, const SDL_Event& sdlEvent, Event& e) -> bool {
    e.type = type;
    e.gdevice.which = sdlEvent.gdevice.which;
    return true;
}

auto PollEvent(Event& e) -> bool {
    e.type = EventType::UNUSED;
    SDL_Event sdlEvent;
    if (!SDL_PollEvent(&sdlEvent)) {
        return false;
    }
    switch (sdlEvent.type) {
        case SDL_EVENT_QUIT:
            return MapQuitEvent(EventType::QUIT, sdlEvent, e);

        case SDL_EVENT_WINDOW_RESIZED:
            return MapWindowEvent(EventType::WINDOW_RESIZED, sdlEvent, e);

        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            return MapWindowEvent(EventType::WINDOW_FOCUS_GAINED, sdlEvent, e);

        case SDL_EVENT_WINDOW_FOCUS_LOST:
            return MapWindowEvent(EventType::WINDOW_FOCUS_LOST, sdlEvent, e);

        case SDL_EVENT_KEY_DOWN:
            return MapKeyEvent(EventType::KEY_DOWN, sdlEvent, e);

        case SDL_EVENT_KEY_UP:
            return MapKeyEvent(EventType::KEY_UP, sdlEvent, e);

        case SDL_EVENT_MOUSE_MOTION:
            return MapMouseMotionEvent(sdlEvent, e);

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            return MapMouseButtonEvent(EventType::MOUSE_BUTTON_DOWN, sdlEvent, e);

        case SDL_EVENT_MOUSE_BUTTON_UP:
            return MapMouseButtonEvent(EventType::MOUSE_BUTTON_UP, sdlEvent, e);

        case SDL_EVENT_GAMEPAD_ADDED:
            return MapGamepadDeviceEvent(EventType::GAMEPAD_ADDED, sdlEvent, e);

        case SDL_EVENT_GAMEPAD_REMOVED:
            return MapGamepadDeviceEvent(EventType::GAMEPAD_REMOVED, sdlEvent, e);

        default: ;
    }
    return true;
}

}
