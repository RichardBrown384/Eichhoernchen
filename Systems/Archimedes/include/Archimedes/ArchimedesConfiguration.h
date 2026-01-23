#pragma once

#include <optional>

namespace rbrown::platform {
enum class ScanCode : int;
enum class MouseButton : int;
}

namespace rbrown::acorn::archimedes {

class ArchimedesKeyboardConfiguration final {
public:
    ArchimedesKeyboardConfiguration() = delete;
    static auto MapScanCode(platform::ScanCode) -> std::optional<uint8_t>;
    static auto MapMouseButton(platform::MouseButton) -> std::optional<uint8_t>;
    static auto MapMouseMotion(float, float) -> std::pair<uint8_t, uint8_t>;
};

}