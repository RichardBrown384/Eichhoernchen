#pragma once

#include "Archimedes/WDC1772.h"

#include <vector>
#include <memory>
#include <cstdint>

namespace rbrown::acorn::archimedes {
auto CreateDiscImage(std::vector<uint8_t>) -> std::unique_ptr<DiscImage>;
}