#include "Frontend/Host/FrontendInput.h"

#include "Common/Platform/Platform.h"
#include "Frontend/InputBindingUtil.h"
#include "Frontend/Toolkit.h"

using ScanCode = rbrown::platform::ScanCode;
using GamepadButton = rbrown::platform::GamepadButton;
using Player = rbrown::platform::Player;

using namespace rbrown::frontend::binding;

constexpr std::array PLAYER_1_KEYBOARD_BINDING = {
    KV(GamepadButton::GAMEPAD_UNKNOWN, ScanCode::CODE_UNKNOWN),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_UP, ScanCode::CODE_W),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_LEFT, ScanCode::CODE_A),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_DOWN, ScanCode::CODE_S),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_RIGHT, ScanCode::CODE_D),

    KV(GamepadButton::GAMEPAD_BUTTON_SOUTH, ScanCode::CODE_F),
    KV(GamepadButton::GAMEPAD_BUTTON_EAST, ScanCode::CODE_G),
    KV(GamepadButton::GAMEPAD_BUTTON_NORTH, ScanCode::CODE_T),
    KV(GamepadButton::GAMEPAD_BUTTON_WEST, ScanCode::CODE_R),

    KV(GamepadButton::GAMEPAD_BUTTON_START, ScanCode::CODE_E),
    KV(GamepadButton::GAMEPAD_BUTTON_BACK, ScanCode::CODE_Q),
};

static_assert(IsBijection(PLAYER_1_KEYBOARD_BINDING), "Player 1 keyboard binding must be a bijection");
static_assert(PLAYER_1_KEYBOARD_BINDING.size() == static_cast<size_t>(GamepadButton::GAMEPAD_MAX),
              "Player 1 keyboard binding must map all logical buttons to logical keys");

constexpr std::array PLAYER_2_KEYBOARD_BINDING = {
    KV(GamepadButton::GAMEPAD_UNKNOWN, ScanCode::CODE_UNKNOWN),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_UP, ScanCode::CODE_I),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_LEFT, ScanCode::CODE_J),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_DOWN, ScanCode::CODE_K),
    KV(GamepadButton::GAMEPAD_BUTTON_DPAD_RIGHT, ScanCode::CODE_L),

    KV(GamepadButton::GAMEPAD_BUTTON_SOUTH, ScanCode::CODE_SEMICOLON),
    KV(GamepadButton::GAMEPAD_BUTTON_EAST, ScanCode::CODE_APOSTROPHE),
    KV(GamepadButton::GAMEPAD_BUTTON_NORTH, ScanCode::CODE_LEFTBRACKET),
    KV(GamepadButton::GAMEPAD_BUTTON_WEST, ScanCode::CODE_P),

    KV(GamepadButton::GAMEPAD_BUTTON_START, ScanCode::CODE_O),
    KV(GamepadButton::GAMEPAD_BUTTON_BACK, ScanCode::CODE_U),
};

static_assert(IsBijection(PLAYER_2_KEYBOARD_BINDING), "Player 2 keyboard binding must be a bijection");
static_assert(PLAYER_2_KEYBOARD_BINDING.size() == static_cast<size_t>(GamepadButton::GAMEPAD_MAX),
              "Player 2 keyboard binding must map all logical buttons to logical keys");

constexpr std::array PLAYER_FORWARD_LOOKUPS = {
    MakeForwardLookup(PLAYER_1_KEYBOARD_BINDING, ScanCode::CODE_UNKNOWN),
    MakeForwardLookup(PLAYER_2_KEYBOARD_BINDING, ScanCode::CODE_UNKNOWN)
};

static_assert(PLAYER_FORWARD_LOOKUPS.size() == static_cast<size_t>(Player::Max));

namespace rbrown::frontend::input {
auto GetKeyboardState(const ScanCode scanCode) -> bool {
    return toolkit::GetKeyboardState(scanCode);
}

auto GetGamepadButton(const Player player, const GamepadButton button) -> bool {
    return toolkit::GetGamepadButton(player, button) || GetKeyboardState(PLAYER_FORWARD_LOOKUPS[U(player)][U(button)]);
}
}
