#pragma once

#include <string>
#include <vector>

namespace rbrown::config {
struct SystemConfiguration {
    int m_scale {};
    std::string m_filename {};
    std::vector<std::string> m_zipSearchExtensions {};
    std::vector<std::string> m_biosFiles {};
};
}