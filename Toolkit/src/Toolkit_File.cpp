#include "Toolkit/Toolkit.h"

#include <fstream>

namespace rbrown::toolkit {

std::vector<uint8_t> LoadFile(const std::filesystem::path& path) {
    std::vector<uint8_t> bytes;
    std::ifstream input(path, std::ios::binary);
    if (input) {
        bytes.insert(bytes.end(),
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()
        );
    }
    return bytes;
}

std::vector<uint8_t> LoadMultipartFile(const std::vector<std::filesystem::path>& paths) {
    std::vector<uint8_t> bytes;
    for (const auto& path: paths) {
        const auto& part = LoadFile(path);
        bytes.insert(bytes.end(), part.begin(), part.end());
    }
    return bytes;
}

std::vector<uint8_t> LoadMultipartFile(const std::vector<std::string>& filenames) {
    std::vector<uint8_t> bytes;
    for (const auto& filename: filenames) {
        const auto& part = LoadFile(filename);
        bytes.insert(bytes.end(), part.begin(), part.end());
    }
    return bytes;
}
}