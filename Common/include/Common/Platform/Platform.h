#pragma once

#include <string>
#include <vector>
#include <functional>

namespace rbrown::platform {

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
    CODE_KP_COMMA,

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

enum class Player : int {
    Player1,
    Player2,
    Max
};

enum class GamepadButton : int {
    GAMEPAD_UNKNOWN,

    GAMEPAD_BUTTON_DPAD_UP,
    GAMEPAD_BUTTON_DPAD_DOWN,
    GAMEPAD_BUTTON_DPAD_LEFT,
    GAMEPAD_BUTTON_DPAD_RIGHT,

    GAMEPAD_BUTTON_NORTH,
    GAMEPAD_BUTTON_SOUTH,
    GAMEPAD_BUTTON_EAST,
    GAMEPAD_BUTTON_WEST,

    GAMEPAD_BUTTON_START,
    GAMEPAD_BUTTON_BACK,

    GAMEPAD_MAX
};

enum class MouseButton: int {
    UNKNOWN,
    LEFT,
    MIDDLE,
    RIGHT,
    SIDE_1,
    SIDE_2,
    MAX
};

struct MouseMotionData {
    float x;
    float y;
    float xrel;
    float yrel;
};

class Host {
public:
    virtual ~Host() = default;

    // Disk I/O interactions
    virtual auto LoadOperatingSystem() -> std::vector<uint8_t> = 0;
    virtual auto LoadNonVolatileRam() -> std::vector<uint8_t> = 0;
    virtual auto LoadMedia(const std::vector<std::string>&) -> std::vector<uint8_t> = 0;

    virtual auto SaveNonVolatileRam(const std::vector<uint8_t>&) -> void = 0;

    // Keyboard interactions
    [[nodiscard]] virtual auto GetKeyboardState(ScanCode) const -> bool = 0;

    // Gamepad interactions
    [[nodiscard]] virtual auto GetGamepadState(Player, GamepadButton) const -> bool = 0;

    // Graphics interactions
    virtual auto SetTextureSource(uint32_t, uint32_t, uint32_t, uint32_t) -> void = 0;
    virtual auto SetPixel(uint32_t x, uint32_t y, uint32_t c) -> void = 0;
    virtual auto GetPixel(uint32_t x, uint32_t y) -> uint32_t = 0;

    // Start a frame
    virtual auto StartProducingFrame() -> void  = 0;

    // Is a frame production in progress
    [[nodiscard]] virtual auto IsProducingFrame() const -> bool = 0;

    // Signal that a frame has been completed
    virtual auto StopProducingFrame() -> void = 0;
};

class Emulator {
public:
    virtual ~Emulator() = default;
    virtual auto StartUp() -> void {}
    virtual auto ShutDown() -> void {}
    virtual auto Reset() -> void {}

    [[nodiscard]] virtual auto FrameTimeMillis() const -> int = 0;
    virtual auto OnFrame() -> void {}
    virtual auto Step() -> void = 0;
    virtual auto KeyUp(ScanCode) -> void {}
    virtual auto KeyDown(ScanCode) -> void {}
    virtual auto MouseMotion(const MouseMotionData&) -> void {}
    virtual auto MouseButtonUp(MouseButton) -> void {}
    virtual auto MouseButtonDown(MouseButton) -> void {}
};


// Machine definition types
struct DisplayProperties {
    std::string windowTitle;
    int logicalWidth{};
    int logicalHeight{};
    int textureWidth{};
    int textureHeight{};
};

struct InputProperties {
    bool needsRelativeMouseInput{};
};

struct SystemProperties {
    DisplayProperties displayProperties;
    InputProperties inputProperties;
    std::function<
        std::unique_ptr<Emulator>(Host&)
    > createEmulator;
};

}
