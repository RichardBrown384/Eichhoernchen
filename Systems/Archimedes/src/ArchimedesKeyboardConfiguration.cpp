#include "Archimedes/ArchimedesConfiguration.h"

#include "Toolkit/Toolkit.h"

#include <array>

using namespace rbrown::toolkit;

namespace rbrown::acorn::archimedes {

constexpr static auto CODE_NONE = 0xFFu;

template<typename T>
constexpr auto U(const T& v) { return std::to_underlying(v); }

constexpr auto MapMouseMotion(float x) -> uint8_t {
    return std::clamp(static_cast<int>(x), -64, 63) & 0x7Fu;
}

consteval auto CreateScanCodeMapping() {
    std::array<uint8_t, U(ScanCode::CODE_MAX)> mapping{};

    mapping.fill(CODE_NONE);

    mapping[U(ScanCode::CODE_ESCAPE)] = 0x00;

    mapping[U(ScanCode::CODE_F1)] = 0x01;
    mapping[U(ScanCode::CODE_F2)] = 0x02;
    mapping[U(ScanCode::CODE_F3)] = 0x03;
    mapping[U(ScanCode::CODE_F4)] = 0x04;
    mapping[U(ScanCode::CODE_F5)] = 0x05;
    mapping[U(ScanCode::CODE_F6)] = 0x06;
    mapping[U(ScanCode::CODE_F7)] = 0x07;
    mapping[U(ScanCode::CODE_F8)] = 0x08;
    mapping[U(ScanCode::CODE_F9)] = 0x09;
    mapping[U(ScanCode::CODE_F10)] = 0x0A;
    mapping[U(ScanCode::CODE_F11)] = 0x0B;
    mapping[U(ScanCode::CODE_F12)] = 0x0C;

    mapping[U(ScanCode::CODE_PRINTSCREEN)] = 0x0D;
    mapping[U(ScanCode::CODE_SCROLLLOCK)] = 0x0E;
    mapping[U(ScanCode::CODE_PAUSE)] = 0x0F;

    mapping[U(ScanCode::CODE_GRAVE)] = 0x10;
    mapping[U(ScanCode::CODE_1)] = 0x11;
    mapping[U(ScanCode::CODE_2)] = 0x12;
    mapping[U(ScanCode::CODE_3)] = 0x13;
    mapping[U(ScanCode::CODE_4)] = 0x14;
    mapping[U(ScanCode::CODE_5)] = 0x15;
    mapping[U(ScanCode::CODE_6)] = 0x16;
    mapping[U(ScanCode::CODE_7)] = 0x17;
    mapping[U(ScanCode::CODE_8)] = 0x18;
    mapping[U(ScanCode::CODE_9)] = 0x19;
    mapping[U(ScanCode::CODE_0)] = 0x1A;
    mapping[U(ScanCode::CODE_MINUS)] = 0x1B;
    mapping[U(ScanCode::CODE_EQUALS)] = 0x1C;
    // TODO POUND SYMBOL
    mapping[U(ScanCode::CODE_BACKSPACE)] = 0x1E;

    mapping[U(ScanCode::CODE_INSERT)] = 0x1F;
    mapping[U(ScanCode::CODE_HOME)] = 0x20;
    mapping[U(ScanCode::CODE_PAGEUP)] = 0x21;

    mapping[U(ScanCode::CODE_NUMLOCKCLEAR)] = 0x22;
    mapping[U(ScanCode::CODE_KP_DIVIDE)] = 0x23;
    mapping[U(ScanCode::CODE_KP_MULTIPLY)] = 0x24;
    // TODO HASH SYMBOL

    mapping[U(ScanCode::CODE_TAB)] = 0x26;
    mapping[U(ScanCode::CODE_Q)] = 0x27;
    mapping[U(ScanCode::CODE_W)] = 0x28;
    mapping[U(ScanCode::CODE_E)] = 0x29;
    mapping[U(ScanCode::CODE_R)] = 0x2A;
    mapping[U(ScanCode::CODE_T)] = 0x2B;
    mapping[U(ScanCode::CODE_Y)] = 0x2C;
    mapping[U(ScanCode::CODE_U)] = 0x2D;
    mapping[U(ScanCode::CODE_I)] = 0x2E;
    mapping[U(ScanCode::CODE_O)] = 0x2F;
    mapping[U(ScanCode::CODE_P)] = 0x30;
    mapping[U(ScanCode::CODE_LEFTBRACKET)] = 0x31;
    mapping[U(ScanCode::CODE_RIGHTBRACKET)] = 0x32;
    mapping[U(ScanCode::CODE_BACKSLASH)] = 0x33u;

    mapping[U(ScanCode::CODE_DELETE)] = 0x34;
    mapping[U(ScanCode::CODE_END)] = 0x35;
    mapping[U(ScanCode::CODE_PAGEDOWN)] = 0x36;

    mapping[U(ScanCode::CODE_KP_7)] = 0x37;
    mapping[U(ScanCode::CODE_KP_8)] = 0x38;
    mapping[U(ScanCode::CODE_KP_9)] = 0x39;
    mapping[U(ScanCode::CODE_KP_MINUS)] = 0x3A;

    mapping[U(ScanCode::CODE_LCTRL)] = 0x3B;
    mapping[U(ScanCode::CODE_A)] = 0x3C;
    mapping[U(ScanCode::CODE_S)] = 0x3D;
    mapping[U(ScanCode::CODE_D)] = 0x3E;
    mapping[U(ScanCode::CODE_F)] = 0x3F;
    mapping[U(ScanCode::CODE_G)] = 0x40;
    mapping[U(ScanCode::CODE_H)] = 0x41;
    mapping[U(ScanCode::CODE_J)] = 0x42;
    mapping[U(ScanCode::CODE_K)] = 0x43;
    mapping[U(ScanCode::CODE_L)] = 0x44;
    mapping[U(ScanCode::CODE_SEMICOLON)] = 0x45;
    mapping[U(ScanCode::CODE_APOSTROPHE)] = 0x46;
    mapping[U(ScanCode::CODE_RETURN)] = 0x47;

    mapping[U(ScanCode::CODE_KP_4)] = 0x48;
    mapping[U(ScanCode::CODE_KP_5)] = 0x49;
    mapping[U(ScanCode::CODE_KP_6)] = 0x4A;
    mapping[U(ScanCode::CODE_KP_PLUS)] = 0x4B;

    mapping[U(ScanCode::CODE_LSHIFT)] = 0x4C;
    mapping[U(ScanCode::CODE_Z)] = 0x4E;
    mapping[U(ScanCode::CODE_X)] = 0x4F;
    mapping[U(ScanCode::CODE_C)] = 0x50;
    mapping[U(ScanCode::CODE_V)] = 0x51;
    mapping[U(ScanCode::CODE_B)] = 0x52;
    mapping[U(ScanCode::CODE_N)] = 0x53;
    mapping[U(ScanCode::CODE_M)] = 0x54;
    mapping[U(ScanCode::CODE_COMMA)] = 0x55;
    mapping[U(ScanCode::CODE_PERIOD)] = 0x56;
    mapping[U(ScanCode::CODE_SLASH)] = 0x57;
    mapping[U(ScanCode::CODE_RSHIFT)] = 0x58u;

    mapping[U(ScanCode::CODE_UP)] = 0x59;

    mapping[U(ScanCode::CODE_KP_1)] = 0x5A;
    mapping[U(ScanCode::CODE_KP_2)] = 0x5B;
    mapping[U(ScanCode::CODE_KP_3)] = 0x5C;

    mapping[U(ScanCode::CODE_CAPSLOCK)] = 0x5D;
    mapping[U(ScanCode::CODE_LALT)] = 0x5E;
    mapping[U(ScanCode::CODE_SPACE)] = 0x5F;
    mapping[U(ScanCode::CODE_RALT)] = 0x60;
    mapping[U(ScanCode::CODE_RCTRL)] = 0x61;

    mapping[U(ScanCode::CODE_LEFT)] = 0x62;
    mapping[U(ScanCode::CODE_DOWN)] = 0x63;
    mapping[U(ScanCode::CODE_RIGHT)] = 0x64;

    mapping[U(ScanCode::CODE_KP_0)] = 0x65;
    mapping[U(ScanCode::CODE_KP_PERIOD)] = 0x66;
    mapping[U(ScanCode::CODE_KP_ENTER)] = 0x67;

    return mapping;
}

consteval auto CreateMouseButtonMapping() {
    std::array<uint8_t, U(MouseButton::MAX)> mapping {};
    mapping.fill(CODE_NONE);
    mapping[U(MouseButton::LEFT)] = 0x70u;
    mapping[U(MouseButton::MIDDLE)] = 0x71u;
    mapping[U(MouseButton::RIGHT)] = 0x72u;
    return mapping;
}

constexpr auto SCAN_CODE_MAPPING = CreateScanCodeMapping();
constexpr auto MOUSE_BUTTON_MAPPING = CreateMouseButtonMapping();

auto ArchimedesKeyboardConfiguration::MapScanCode(ScanCode k) -> std::optional<uint8_t> {
    return (SCAN_CODE_MAPPING[U(k)] == CODE_NONE) ?
           std::nullopt :
           std::optional<uint8_t>{ SCAN_CODE_MAPPING[U(k)] };
}

auto ArchimedesKeyboardConfiguration::MapMouseButton(MouseButton b) -> std::optional<uint8_t> {
    return (MOUSE_BUTTON_MAPPING[U(b)] == CODE_NONE) ?
           std::nullopt :
           std::optional<uint8_t>{ MOUSE_BUTTON_MAPPING[U(b)] };
}

auto ArchimedesKeyboardConfiguration::MapMouseMotion(float x, float y) -> std::pair<uint8_t, uint8_t> {
    return { archimedes::MapMouseMotion(x), archimedes::MapMouseMotion(-y) };
}

}