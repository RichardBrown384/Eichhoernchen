#include "Toolkit/Toolkit.h"

#include <algorithm>
#include <memory>

// Note: ziplib's CMakeLists.txt compiles c code to check
// if the compiler supports _Nullable and _Nonnull, which it does,
// but when we come to compile that's an error because it's an extension
#define _Nullable
#define _Nonnull
#include <zip.h>
#undef _Nullable
#undef _Nonnull

constexpr auto READ_BUFFER_SIZE = 4096;

template<typename ...Args>
constexpr auto ZipOpen(Args&&... args) {
    return std::unique_ptr<zip_t, decltype(&zip_discard)>
        (zip_open(std::forward<Args>(args)...), zip_discard);
}

template<typename ...Args>
constexpr auto ZipFOpenIndex(Args&&... args) {
    return std::unique_ptr<zip_file_t, decltype(&zip_fclose)>
        (zip_fopen_index(std::forward<Args>(args)...), zip_fclose);
}

constexpr bool HasSuffix(const std::string& name, const std::vector<std::string>& suffixes) {
    return std::ranges::any_of(suffixes, [&name](const std::string& suffix) {
        return name.ends_with(suffix);
    });
}

namespace rbrown::toolkit {
auto PathToString(const std::filesystem::path& path) -> std::string {
    const auto u8path = path.generic_u8string();
    return {u8path.begin(), u8path.end()};
}

auto FindEntry(zip_t& zip, const std::function<bool(std::string)>& predicate) -> zip_int64_t {
    const auto entries = zip_get_num_entries(&zip, 0);
    for (zip_int64_t entry = 0; entry < entries; ++entry) {
        zip_stat_t zip_stat;
        zip_stat_init(&zip_stat);
        if (!zip_stat_index(&zip, entry, 0, &zip_stat) && predicate(zip_stat.name)) {
            return entry;
        }
    }
    return -1;
}

auto UnzipEntry(zip_t &zip, zip_int64_t entry) -> std::vector<uint8_t> {
    const auto file = ZipFOpenIndex(&zip, entry, 0);
    if (!file) {
        return {};
    }
    std::vector<uint8_t> buffer(READ_BUFFER_SIZE);
    std::vector<uint8_t> data;
    while (true) {
        const auto count = zip_fread(file.get(), buffer.data(), READ_BUFFER_SIZE);
        if (count < 0) {
            data.clear();
            break;
        }
        if (count > 0) {
            data.insert(data.end(),
                        buffer.begin(),
                        buffer.begin() + count
            );
        }
        if (count < READ_BUFFER_SIZE) {
            break;
        }
    }
    return data;
}

std::vector<uint8_t>
LoadZippedFile(const std::filesystem::path& path, const std::function<bool(std::string)>& predicate) {
    int zip_error;
    const auto pathString = PathToString(path);
    if (const auto zip = ZipOpen(pathString.c_str(), ZIP_RDONLY, &zip_error)) {
        const auto entry = FindEntry(*zip, predicate);
        if (entry >= 0) {
            return UnzipEntry(*zip, entry);
        }
    }
    return {};
}

std::vector<uint8_t> LoadZippedFile(const std::filesystem::path& path, const std::vector<std::string>& extensions) {
    const auto predicate = [&](const std::string& entry) {
        return HasSuffix(entry, extensions);
    };
    return LoadZippedFile(path, predicate);
}

std::vector<uint8_t> LoadZippedFileWithFallback(const std::filesystem::path& path, const std::function<bool(std::string)>& predicate) {
    int zip_error;
    const auto pathString = PathToString(path);
    if (const auto zip = ZipOpen(pathString.c_str(), ZIP_RDONLY, &zip_error)) {
        const auto entry = FindEntry(*zip, predicate);
        if (entry >= 0) {
            return UnzipEntry(*zip, entry);
        }
        return {};
    }
    return LoadFile(path);
}

std::vector<uint8_t> LoadZippedFileWithFallback(const std::filesystem::path& path, const std::vector<std::string>& extensions) {
    const auto predicate = [&](const std::string& entry) {
        return HasSuffix(entry, extensions);
    };
    return LoadZippedFileWithFallback(path, predicate);
}
}