#pragma once

#include "Common/Platform/Platform.h"

#include <filesystem>
#include <string>

namespace rbrown::frontend {
struct ProgramArguments {
    std::string system;
    std::filesystem::path mediaPath;
    std::filesystem::path dataDirectory;
    int scale{};
};

auto ParseArguments(int argc, const char** argv) -> ProgramArguments;

}