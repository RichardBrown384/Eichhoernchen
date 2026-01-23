#pragma once

#include "Common/Platform/Platform.h"

#include <memory>
#include <filesystem>

namespace rbrown::toolkit {
class Texture;
}

namespace rbrown::frontend {
struct HostOptions {
    std::filesystem::path mediaPath;
    std::filesystem::path dataDirectory;
};

auto CreateHost(const HostOptions&, toolkit::Texture&) -> std::unique_ptr<platform::Host>;
}
