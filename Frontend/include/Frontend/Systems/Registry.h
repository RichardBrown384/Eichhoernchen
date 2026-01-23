#pragma once

#include "Common/Platform/Platform.h"

#include <ranges>
#include <string>
#include <unordered_map>

namespace rbrown::frontend {
class Registry {
public:
    Registry() = default;

    Registry(const Registry&) = delete;

    Registry& operator=(const Registry&) = delete;

    Registry(Registry&&) = delete;

    Registry& operator=(Registry&&) = delete;

    auto Register(const std::string& name, const platform::SystemProperties& properties) -> void {
        m_registry[name] = properties;
    }

    [[nodiscard]] auto Get(const std::string& name) const -> std::optional<platform::SystemProperties> {
        if (const auto it = m_registry.find(name); it != m_registry.end()) {
            return it->second;
        }
        return std::nullopt;
    }

private:
    std::unordered_map<std::string, platform::SystemProperties> m_registry;
};
}
