#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <functional>

namespace rbrown::frontend::filesystem {
auto LoadFile(const std::filesystem::path&) -> std::vector<uint8_t>;

auto LoadMultipartFile(const std::vector<std::filesystem::path>&) -> std::vector<uint8_t>;

auto LoadMultipartFileFromDirectory(const std::filesystem::path&) -> std::vector<uint8_t>;

auto LoadZippedFile(const std::filesystem::path&,
                    const std::function<bool(std::string)>&) -> std::vector<uint8_t>;

auto LoadZippedFile(const std::filesystem::path&, const std::vector<std::string>&) -> std::vector<uint8_t>;

auto LoadZippedFileWithFallback(const std::filesystem::path&,
                                const std::function<bool(std::string)>&) -> std::vector<uint8_t>;

auto LoadZippedFileWithFallback(const std::filesystem::path&,
                                const std::vector<std::string>&) -> std::vector<uint8_t>;

auto SaveFile(const std::filesystem::path&, const std::vector<uint8_t>&) -> void;
}
