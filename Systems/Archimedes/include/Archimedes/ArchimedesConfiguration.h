#pragma once

#include "Common/Config/Config.h"

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace rbrown::toolkit {
enum class ScanCode : int;
enum class MouseButton : int;
}

namespace rbrown::acorn::archimedes {

class ArchimedesKeyboardConfiguration final {
public:
    ArchimedesKeyboardConfiguration() = delete;
    static auto MapScanCode(rbrown::toolkit::ScanCode) -> std::optional<uint8_t>;
    static auto MapMouseButton(rbrown::toolkit::MouseButton) -> std::optional<uint8_t>;
    static auto MapMouseMotion(float, float) -> std::pair<uint8_t, uint8_t>;
};

struct ArchimedesConfiguration {
    rbrown::config::SystemConfiguration m_config {};
};

}