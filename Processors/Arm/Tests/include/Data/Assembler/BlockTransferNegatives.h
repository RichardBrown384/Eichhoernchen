#pragma once

#include "Data/Assembler/AssemblerNegativeParameter.h"

constexpr AssemblerNegativeParameter ASSEMBLER_BLOCK_TRANSFER_NEGATIVE_VALUES[] = {
    {
        .name = "Register list should not be empty",
        .assembly = "LDMIA R2, {}"
    }
};
