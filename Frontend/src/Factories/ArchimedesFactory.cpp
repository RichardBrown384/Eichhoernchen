#include "Archimedes/Archimedes.h"
#include "Archimedes/ArchimedesConfiguration.h"

#include "Common/Config/Config.h"

#include "Toolkit/Toolkit.h"

using namespace rbrown::config;
using namespace rbrown::toolkit;

auto CreateArchimedes(const SystemConfiguration& config) -> std::unique_ptr<Emulator> {
    return rbrown::acorn::archimedes::CreateArchimedes({
        .m_config = config
    });
}
