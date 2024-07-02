#pragma once

#include "Archimedes/ArchimedesConfiguration.h"

#include "Toolkit/Toolkit.h"

#include <memory>

namespace rbrown::acorn::archimedes {
auto CreateArchimedes(const ArchimedesConfiguration&) -> std::unique_ptr<rbrown::toolkit::Emulator>;
}
