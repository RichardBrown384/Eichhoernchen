#pragma once

#include "Data/Assembler/AssemblerNegativeParameter.h"

constexpr AssemblerNegativeParameter ASSEMBLER_SINGLE_DATA_TRANSFER_NEGATIVE_VALUES[] = {
    {
        .name = "Pre-indexed register shift",
        .assembly = "LDR R2, [R3, R4, ROR R3]"
    },
    {
        .name = "Post-indexed register shift",
        .assembly = "LDR R2, [R3], R4, ROR R3"
    }
};
