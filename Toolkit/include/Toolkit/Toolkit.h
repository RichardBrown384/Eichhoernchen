#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <functional>

namespace rbrown::toolkit {

struct FloatRect {
    float x, y, w, h;
};

class Texture;

class Window {
public:
    virtual ~Window() = default;
    virtual auto SetSize(int, int) -> bool = 0;
};

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual auto RenderTexture(Texture&, const FloatRect&) -> bool = 0;
    virtual auto Present() -> bool = 0;
};

class Texture {
public:
    virtual ~Texture() = default;
    virtual auto LockTexture() -> bool = 0;
    virtual auto UnlockTexture() -> void = 0;
    virtual auto SetPixel(int x, int y, uint32_t c) -> void = 0;
    virtual auto GetPixel(int x, int y) -> uint32_t = 0;
};

class PresentationContext {
public:
    virtual ~PresentationContext() = default;
    virtual auto SetTextureSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void = 0;
    virtual auto SetWindowSize(uint32_t, uint32_t) -> void = 0;
    virtual auto LockTexture() -> bool = 0;
    virtual auto UnlockTexture() -> void = 0;
    virtual auto Present() -> void = 0;
    virtual auto SetPixel(uint32_t x, uint32_t y, uint32_t c) -> void = 0;
    virtual auto GetPixel(uint32_t x, uint32_t y) -> uint32_t = 0;
    virtual auto StartFrame() -> void = 0;
    virtual auto CompleteFrame() -> void = 0;
    [[nodiscard]] virtual auto IsFrameComplete() const -> bool = 0;
};

struct KeyboardEvent;
struct MouseMotionEvent;
struct MouseButtonEvent;

class Emulator {
public:
    virtual ~Emulator() = default;
    virtual auto StartUp() -> void {};
    virtual auto ShutDown() -> void {};
    virtual auto Reset() -> void = 0;
    virtual auto RunForFrame() -> void = 0;
    virtual auto KeyUp(const KeyboardEvent&) -> void {};
    virtual auto KeyDown(const KeyboardEvent&) -> void {};
    virtual auto MouseMotion(const MouseMotionEvent&) -> void {}
    virtual auto MouseButtonUp(const MouseButtonEvent&) -> void {}
    virtual auto MouseButtonDown(const MouseButtonEvent&) -> void {}
};

enum class ScanCode : int {
    CODE_UNKNOWN,

    CODE_A,
    CODE_B,
    CODE_C,
    CODE_D,
    CODE_E,
    CODE_F,
    CODE_G,
    CODE_H,
    CODE_I,
    CODE_J,
    CODE_K,
    CODE_L,
    CODE_M,
    CODE_N,
    CODE_O,
    CODE_P,
    CODE_Q,
    CODE_R,
    CODE_S,
    CODE_T,
    CODE_U,
    CODE_V,
    CODE_W,
    CODE_X,
    CODE_Y,
    CODE_Z,

    CODE_1,
    CODE_2,
    CODE_3,
    CODE_4,
    CODE_5,
    CODE_6,
    CODE_7,
    CODE_8,
    CODE_9,
    CODE_0,

    CODE_RETURN,
    CODE_ESCAPE,
    CODE_BACKSPACE,
    CODE_TAB,
    CODE_SPACE,

    CODE_MINUS,
    CODE_EQUALS,
    CODE_LEFTBRACKET,
    CODE_RIGHTBRACKET,
    CODE_BACKSLASH,
    CODE_SEMICOLON,
    CODE_APOSTROPHE,
    CODE_GRAVE,
    CODE_COMMA,
    CODE_PERIOD,
    CODE_SLASH,

    CODE_CAPSLOCK,

    CODE_F1,
    CODE_F2,
    CODE_F3,
    CODE_F4,
    CODE_F5,
    CODE_F6,
    CODE_F7,
    CODE_F8,
    CODE_F9,
    CODE_F10,
    CODE_F11,
    CODE_F12,
    
    CODE_PRINTSCREEN,
    CODE_SCROLLLOCK,
    CODE_PAUSE,
    CODE_INSERT,

    CODE_HOME,
    CODE_PAGEUP,
    CODE_DELETE,
    CODE_END,
    CODE_PAGEDOWN,
    CODE_RIGHT,
    CODE_LEFT,
    CODE_DOWN,
    CODE_UP,

    CODE_NUMLOCKCLEAR,

    CODE_KP_DIVIDE,
    CODE_KP_MULTIPLY,
    CODE_KP_MINUS,
    CODE_KP_PLUS,
    CODE_KP_ENTER,
    CODE_KP_1,
    CODE_KP_2,
    CODE_KP_3,
    CODE_KP_4,
    CODE_KP_5,
    CODE_KP_6,
    CODE_KP_7,
    CODE_KP_8,
    CODE_KP_9,
    CODE_KP_0,
    CODE_KP_PERIOD,

    CODE_LCTRL,
    CODE_LSHIFT,
    CODE_LALT,
    CODE_LGUI,
    CODE_RCTRL,
    CODE_RSHIFT,
    CODE_RALT,
    CODE_RGUI,

    CODE_MAX
};

enum class MouseButton: int {
    UNKNOWN,
    LEFT,
    MIDDLE,
    RIGHT,
    MAX
};

enum class EventType: int {
    UNUSED,
    QUIT,
    KEY_UP,
    KEY_DOWN,
    MOUSE_MOTION,
    MOUSE_BUTTON_UP,
    MOUSE_BUTTON_DOWN,
};

struct KeyboardEvent {
    uint8_t repeat;
    ScanCode scanCode;
};

struct MouseMotionEvent {
    float x;
    float y;
    float xrel;
    float yrel;
};

struct MouseButtonEvent {
    MouseButton button;
};

struct Event {
    EventType type;
    KeyboardEvent key;
    MouseMotionEvent motion;
    MouseButtonEvent button;
};

auto Init() -> bool;
auto Quit() -> void;
auto PollEvent(Event&) -> bool;
auto GetKeyboardState(ScanCode) -> bool;
auto SetRelativeMouseMode(bool) -> bool;

auto CreateWindow(const char*, int, int) -> std::unique_ptr<Window>;
auto CreateRenderer(Window&) -> std::unique_ptr<Renderer>;
auto CreateTexture(Renderer&, int, int) -> std::unique_ptr<Texture>;
auto CreatePresentationContext(const char*, int, int, int) -> std::unique_ptr<PresentationContext>;

std::vector<uint8_t> LoadFile(const std::filesystem::path&);
std::vector<uint8_t> LoadMultipartFile(const std::vector<std::filesystem::path>&);
std::vector<uint8_t> LoadMultipartFile(const std::vector<std::string>&);

std::vector<uint8_t> LoadZippedFile(const std::filesystem::path&, const std::function<bool(std::string)>&);
std::vector<uint8_t> LoadZippedFile(const std::filesystem::path&, const std::vector<std::string>&);
std::vector<uint8_t> LoadZippedFileWithFallback(const std::filesystem::path&, const std::function<bool(std::string)>&);
std::vector<uint8_t> LoadZippedFileWithFallback(const std::filesystem::path&, const std::vector<std::string>&);

}