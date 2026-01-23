#pragma once

namespace rbrown::platform {
enum class ScanCode: int;
enum class Player: int;
enum class GamepadButton: int;
}

namespace rbrown::frontend::input {
auto GetKeyboardState(platform::ScanCode) -> bool;
auto GetGamepadButton(platform::Player, platform::GamepadButton) -> bool;
}